//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/main/connection_manager.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/common.hpp"
#include "graindb/common/unordered_set.hpp"

#include <mutex>

namespace graindb {

class GrainDB;
class Connection;

class ConnectionManager {
public:
	~ConnectionManager();

	void AddConnection(Connection *conn);
	void RemoveConnection(Connection *conn);

	template <class T> void Scan(T &&callback) {
		// lock the catalog set
		std::lock_guard<std::mutex> lock(connections_lock);
		for (auto &conn : connections) {
			callback(conn);
		}
	}

private:
	std::mutex connections_lock;
	unordered_set<Connection *> connections;
};

} // namespace graindb
