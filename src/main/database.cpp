#include "graindb/main/database.hpp"

#include "graindb/catalog/catalog.hpp"
#include "graindb/common/file_system.hpp"
#include "graindb/main/connection_manager.hpp"
#include "graindb/storage/storage_manager.hpp"
#include "graindb/transaction/transaction_manager.hpp"

using namespace graindb;
using namespace std;

DBConfig::~DBConfig() {
}

GrainDB::GrainDB(const char *path, DBConfig *config) {
	if (config) {
		// user-supplied configuration
		Configure(*config);
	} else {
		// default configuration
		DBConfig config;
		Configure(config);
	}
	if (temporary_directory.empty() && path) {
		// no directory specified: use default temp path
		temporary_directory = string(path) + ".tmp";

		// special treatment for in-memory mode
		if (strcmp(path, ":memory:") == 0) {
			temporary_directory = ".tmp";
		}
	}
	if (config && !config->use_temporary_directory) {
		// temporary directories explicitly disabled
		temporary_directory = string();
	}

	storage = make_unique<StorageManager>(*this, path ? string(path) : string(), access_mode == AccessMode::READ_ONLY);
	catalog = make_unique<Catalog>(*storage);
	transaction_manager = make_unique<TransactionManager>(*storage);
	connection_manager = make_unique<ConnectionManager>();
	// initialize the database
	storage->Initialize();
}

GrainDB::GrainDB(const string &path, DBConfig *config) : GrainDB(path.c_str(), config) {
}

GrainDB::~GrainDB() {
}

void GrainDB::Configure(DBConfig &config) {
	if (config.access_mode != AccessMode::UNDEFINED) {
		access_mode = config.access_mode;
	} else {
		access_mode = AccessMode::READ_WRITE;
	}
	if (config.file_system) {
		file_system = move(config.file_system);
	} else {
		file_system = make_unique<FileSystem>();
	}
	checkpoint_only = config.checkpoint_only;
	checkpoint_wal_size = config.checkpoint_wal_size;
	use_direct_io = config.use_direct_io;
	maximum_memory = config.maximum_memory;
	temporary_directory = config.temporary_directory;
	collation = config.collation;
}
