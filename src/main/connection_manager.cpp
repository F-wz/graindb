#include "graindb/main/connection_manager.hpp"

#include "graindb/main/client_context.hpp"
#include "graindb/main/connection.hpp"

using namespace graindb;
using namespace std;

ConnectionManager::~ConnectionManager() {
	std::lock_guard<std::mutex> lock(connections_lock);
	for (auto &conn : connections) {
		conn->context->Invalidate();
	}
}

void ConnectionManager::AddConnection(Connection *conn) {
	assert(conn);
	std::lock_guard<std::mutex> lock(connections_lock);
	connections.insert(conn);
}

void ConnectionManager::RemoveConnection(Connection *conn) {
	assert(conn);
	std::lock_guard<std::mutex> lock(connections_lock);
	connections.erase(conn);
}
