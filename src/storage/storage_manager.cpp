#include "graindb/storage/storage_manager.hpp"
#include "graindb/storage/checkpoint_manager.hpp"
#include "graindb/storage/in_memory_block_manager.hpp"
#include "graindb/storage/single_file_block_manager.hpp"

#include "graindb/catalog/catalog.hpp"
#include "graindb/common/file_system.hpp"
#include "graindb/main/database.hpp"
#include "graindb/main/client_context.hpp"
#include "graindb/function/function.hpp"
#include "graindb/parser/parsed_data/create_schema_info.hpp"
#include "graindb/transaction/transaction_manager.hpp"
#include "graindb/planner/binder.hpp"
#include "graindb/common/serializer/buffered_file_reader.hpp"

using namespace graindb;
using namespace std;

StorageManager::StorageManager(GrainDB &db, string path, bool read_only)
    : database(db), path(path), wal(db), read_only(read_only) {
}

StorageManager::~StorageManager() {
}

BufferManager &BufferManager::GetBufferManager(ClientContext &context) {
	return *context.db.storage->buffer_manager;
}

void StorageManager::Initialize() {
	bool in_memory = path.empty() || path == ":memory:";

	if (in_memory && read_only) {
		throw CatalogException("Cannot launch in-memory database in read-only mode!");
	}

	// first initialize the base system catalogs
	// these are never written to the WAL
	ClientContext context(database);
	context.transaction.BeginTransaction();

	// create the default schema
	CreateSchemaInfo info;
	info.schema = DEFAULT_SCHEMA;
	database.catalog->CreateSchema(context, &info);

	// initialize default functions
	BuiltinFunctions builtin(context, *database.catalog);
	builtin.Initialize();

	// commit transactions
	context.transaction.Commit();

	if (!in_memory) {
		// create or load the database from disk, if not in-memory mode
		LoadDatabase();
	} else {
		block_manager = make_unique<InMemoryBlockManager>();
		buffer_manager = make_unique<BufferManager>(*database.file_system, *block_manager, database.temporary_directory,
		                                            database.maximum_memory);
	}
}

void StorageManager::Checkpoint(string wal_path) {
	if (!database.file_system->FileExists(wal_path)) {
		// no WAL to checkpoint
		return;
	}
	if (read_only) {
		// cannot checkpoint in read-only system
		return;
	}
	// check the size of the WAL
	{
		BufferedFileReader reader(*database.file_system, wal_path.c_str());
		if (reader.FileSize() <= database.checkpoint_wal_size) {
			// WAL is too small
			return;
		}
	}

	// checkpoint the database
	// FIXME: we do this now by creating a new database and forcing a checkpoint in that database
	// then reloading the file again
	// this should be fixed and turned into an incremental checkpoint
	DBConfig config;
	config.checkpoint_only = true;
	GrainDB db(path, &config);
}

void StorageManager::LoadDatabase() {
	string wal_path = path + ".wal";
	// first check if the database exists
	if (!database.file_system->FileExists(path)) {
		if (read_only) {
			throw CatalogException("Cannot open database \"%s\" in read-only mode: database does not exist",
			                       path.c_str());
		}
		// check if the WAL exists
		if (database.file_system->FileExists(wal_path)) {
			// WAL file exists but database file does not
			// remove the WAL
			database.file_system->RemoveFile(wal_path);
		}
		// initialize the block manager while creating a new db file
		block_manager =
		    make_unique<SingleFileBlockManager>(*database.file_system, path, read_only, true, database.use_direct_io);
		buffer_manager = make_unique<BufferManager>(*database.file_system, *block_manager, database.temporary_directory,
		                                            database.maximum_memory);
	} else {
		if (!database.checkpoint_only) {
			Checkpoint(wal_path);
		}
		// initialize the block manager while loading the current db file
		auto sf =
		    make_unique<SingleFileBlockManager>(*database.file_system, path, read_only, false, database.use_direct_io);
		buffer_manager = make_unique<BufferManager>(*database.file_system, *sf, database.temporary_directory,
		                                            database.maximum_memory);
		sf->LoadFreeList(*buffer_manager);
		block_manager = move(sf);

		//! Load from storage
		CheckpointManager checkpointer(*this);
		checkpointer.LoadFromStorage();
		// check if the WAL file exists
		if (database.file_system->FileExists(wal_path)) {
			// replay the WAL
			WriteAheadLog::Replay(database, wal_path);
			if (database.checkpoint_only) {
				assert(!read_only);
				// checkpoint the database
				checkpointer.CreateCheckpoint();
				// remove the WAL
				database.file_system->RemoveFile(wal_path);
			}
		}
	}
	// initialize the WAL file
	if (!database.checkpoint_only && !read_only) {
		wal.Initialize(wal_path);
	}
}
