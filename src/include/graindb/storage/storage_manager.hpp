//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/storage/storage_manager.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/helper.hpp"
#include "graindb/storage/buffer_manager.hpp"
#include "graindb/storage/data_table.hpp"
#include "graindb/storage/write_ahead_log.hpp"

namespace graindb {
class BlockManager;
class Catalog;
class GrainDB;
class TransactionManager;
class TableCatalogEntry;

//! StorageManager is responsible for managing the physical storage of the
//! database on disk
class StorageManager {
public:
	StorageManager(GrainDB &database, string path, bool read_only);
	~StorageManager();

	//! Initialize a database or load an existing database from the given path
	void Initialize();
	//! Get the WAL of the StorageManager, returns nullptr if in-memory
	WriteAheadLog *GetWriteAheadLog() {
		return wal.initialized ? &wal : nullptr;
	}

	GrainDB &GetDatabase() {
		return database;
	}
	//! The BlockManager to read/store meta information and data in blocks
	unique_ptr<BlockManager> block_manager;
	//! The BufferManager of the database
	unique_ptr<BufferManager> buffer_manager;
	//! The database this storagemanager belongs to
	GrainDB &database;

private:
	//! Load the database from a directory
	void LoadDatabase();
	//! Create a checkpoint of the database
	void Checkpoint(string wal_path);

	//! The path of the database
	string path;
	//! The WriteAheadLog of the storage manager
	WriteAheadLog wal;

	//! Whether or not the database is opened in read-only mode
	bool read_only;
};

} // namespace graindb
