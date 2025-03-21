#include <chrono>
#include <cstdio>
#include <thread>
#include <iostream>

#include "graindb.hpp"
#include "graindb/common/types/data_chunk.hpp"
#include "graindb/common/vector_operations/vector_operations.hpp"
#include "graindb/common/unordered_map.hpp"
#include "graindb/common/string_util.hpp"
#include "graindb/main/client_context.hpp"

// you can set this to enable compression. You will need to link zlib as well.
// #define CPPHTTPLIB_ZLIB_SUPPORT 1

#include "httplib.hpp"
#include "json.hpp"

#include <unordered_map>

using namespace httplib;
using namespace graindb;
using namespace nlohmann;

void print_help() {
	fprintf(stderr, "🦆 Usage: graindb_rest_server\n");
	fprintf(stderr, "          --listen=[address]    listening address\n");
	fprintf(stderr, "          --port=[no]           listening port\n");
	fprintf(stderr, "          --database=[file]     use given database file\n");
	fprintf(stderr, "          --read_only           open database in read-only mode\n");
	fprintf(stderr, "          --query_timeout=[sec] query timeout in seconds\n");
	fprintf(stderr, "          --fetch_timeout=[sec] result set timeout in seconds\n");
	fprintf(stderr, "          --log=[file]          log queries to file\n\n");
	fprintf(stderr, "Version: %s\n", GRAINDB_SOURCE_ID);

}

// https://stackoverflow.com/a/12468109/2652376
std::string random_string(size_t length) {
	auto randchar = []() -> char {
		const char charset[] = "0123456789"
		                       "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		                       "abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}

struct RestClientState {
	unique_ptr<QueryResult> res;
	unique_ptr<Connection> con;
	time_t touched;
};

enum ReturnContentType { JSON, BSON, CBOR, MESSAGE_PACK, UBJSON };

template <class T, class TARGET> static void assign_json_loop(Vector &v, idx_t col_idx, idx_t count, json &j) {
	auto data_ptr = FlatVector::GetData<T>(v);
	auto &nullmask = FlatVector::Nullmask(v);
	for (idx_t i = 0; i < count; i++) {
		if (!nullmask[i]) {
			j["data"][col_idx] += (TARGET)data_ptr[i];

		} else {
			j["data"][col_idx] += nullptr;
		}
	}
}

void serialize_chunk(QueryResult *res, DataChunk *chunk, json &j) {
	assert(res);
	Vector v2(TypeId::VARCHAR);
	for (size_t col_idx = 0; col_idx < chunk->column_count(); col_idx++) {
		Vector *v = &chunk->data[col_idx];
		switch (res->sql_types[col_idx].id) {
		case SQLTypeId::DATE:
		case SQLTypeId::TIME:
		case SQLTypeId::TIMESTAMP: {
			VectorOperations::Cast(*v, v2, res->sql_types[col_idx], SQLType::VARCHAR, chunk->size());
			v = &v2;
			break;
		}
		default:
			break;
		}
		v->Normalify(chunk->size());
		assert(v);
		switch (v->type) {
		case TypeId::BOOL:
			assign_json_loop<bool, int64_t>(*v, col_idx, chunk->size(), j);
			break;
		case TypeId::INT8:
			assign_json_loop<int8_t, int64_t>(*v, col_idx, chunk->size(), j);
			break;
		case TypeId::INT16:
			assign_json_loop<int16_t, int64_t>(*v, col_idx, chunk->size(), j);
			break;
		case TypeId::INT32:
			assign_json_loop<int32_t, int64_t>(*v, col_idx, chunk->size(), j);
			break;
		case TypeId::INT64:
			assign_json_loop<int64_t, int64_t>(*v, col_idx, chunk->size(), j);
			break;
		case TypeId::FLOAT:
			assign_json_loop<float, double>(*v, col_idx, chunk->size(), j);
			break;
		case TypeId::DOUBLE:
			assign_json_loop<float, double>(*v, col_idx, chunk->size(), j);
			break;
		case TypeId::VARCHAR: {
			auto data_ptr = FlatVector::GetData<string_t>(*v);
			auto &nullmask = FlatVector::Nullmask(*v);
			for (idx_t i = 0; i < chunk->size(); i++) {
				if (!nullmask[i]) {
					j["data"][col_idx] += data_ptr[i].GetData();

				} else {
					j["data"][col_idx] += nullptr;
				}
			}
			break;
		}
		default:
			throw std::runtime_error("Unsupported Type");
		}
	}
}

void serialize_json(const Request &req, Response &resp, json &j) {
	auto return_type = ReturnContentType::JSON;
	j["graindb_version"] = GRAINDB_SOURCE_ID;

	if (req.has_header("Accept")) {
		auto accept = req.get_header_value("Accept");
		if (accept.rfind("application/bson", 0) == 0 || accept.rfind("application/x-bson", 0) == 0) {
			return_type = ReturnContentType::BSON;
		} else if (accept.rfind("application/cbor", 0) == 0) {
			return_type = ReturnContentType::CBOR;
		} else if (accept.rfind("application/msgpack", 0) == 0 || accept.rfind("application/x-msgpack", 0) == 0 ||
		           accept.rfind("application/vnd.msgpack", 0) == 0) {
			return_type = ReturnContentType::MESSAGE_PACK;
		} else if (accept.rfind("application/ubjson", 0) == 0) {
			return_type = ReturnContentType::UBJSON;
		}
	}

	switch (return_type) {
	case ReturnContentType::JSON: {
		if (req.has_param("callback")) {
			auto jsonp_callback = req.get_param_value("callback");
			resp.set_content(jsonp_callback + "(" + j.dump() + ");", "application/javascript");

		} else {
			resp.set_content(j.dump(), "application/json");
		}
		break;
	}
	case ReturnContentType::BSON: {
		auto bson = json::to_bson(j);
		resp.set_content((const char *)bson.data(), bson.size(), "application/bson");
		break;
	}
	case ReturnContentType::CBOR: {
		auto cbor = json::to_cbor(j);
		resp.set_content((const char *)cbor.data(), cbor.size(), "application/cbor");
		break;
	}
	case ReturnContentType::MESSAGE_PACK: {
		auto msgpack = json::to_msgpack(j);
		resp.set_content((const char *)msgpack.data(), msgpack.size(), "application/msgpack");
		break;
	}
	case ReturnContentType::UBJSON: {
		auto ubjson = json::to_ubjson(j);
		resp.set_content((const char *)ubjson.data(), ubjson.size(), "application/ubjson");
		break;
	}
	}
}

void sleep_thread(Connection *conn, bool *is_active, int timeout_duration) {
	// timeout is given in seconds
	// we wait 10ms per iteration, so timeout * 100 gives us the amount of
	// iterations
	assert(conn);
	assert(is_active);

	if (timeout_duration < 0) {
		return;
	}
	for (size_t i = 0; i < (size_t)(timeout_duration * 100) && *is_active; i++) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	if (*is_active) {
		conn->Interrupt();
	}
}

void client_state_cleanup(unordered_map<string, RestClientState> *map, std::mutex *mutex, int timeout_duration) {
	// timeout is given in seconds
	while (true) {
		// sleep for half the timeout duration
		std::this_thread::sleep_for(std::chrono::milliseconds((timeout_duration * 1000) / 2));
		{
			std::lock_guard<std::mutex> guard(*mutex);
			auto now = std::time(nullptr);
			for (auto it = map->cbegin(); it != map->cend();) {
				if (now - it->second.touched > timeout_duration) {
					it = map->erase(it);
				} else {
					++it;
				}
			}
		}
	}
}

int main(int argc, char **argv) {
	Server svr;
	if (!svr.is_valid()) {
		printf("server has an error...\n");
		return -1;
	}

	std::mutex out_mutex;
	srand(time(nullptr));

	DBConfig config;
	string dbfile = "";
	string logfile_name;

	string listen = "localhost";
	int port = 1294;
	std::ofstream logfile;

	int query_timeout = 60;
	int fetch_timeout = 60 * 5;

	// parse config
	for (int arg_index = 1; arg_index < argc; ++arg_index) {
		string arg = argv[arg_index];
		if (arg == "--help") {
			print_help();
			exit(0);
		} else if (arg == "--read_only") {
			config.access_mode = AccessMode::READ_ONLY;
		} else if (StringUtil::StartsWith(arg, "--database=")) {
			auto splits = StringUtil::Split(arg, '=');
			if (splits.size() != 2) {
				print_help();
				exit(1);
			}
			dbfile = string(splits[1]);
		} else if (StringUtil::StartsWith(arg, "--log=")) {
			auto splits = StringUtil::Split(arg, '=');
			if (splits.size() != 2) {
				print_help();
				exit(1);
			}
			logfile_name = string(splits[1]);
		} else if (StringUtil::StartsWith(arg, "--listen=")) {
			auto splits = StringUtil::Split(arg, '=');
			if (splits.size() != 2) {
				print_help();
				exit(1);
			}
			listen = string(splits[1]);
		} else if (StringUtil::StartsWith(arg, "--port=")) {
			auto splits = StringUtil::Split(arg, '=');
			if (splits.size() != 2) {
				print_help();
				exit(1);
			}
			port = std::stoi(splits[1]);

		} else if (StringUtil::StartsWith(arg, "--query_timeout=")) {
			auto splits = StringUtil::Split(arg, '=');
			if (splits.size() != 2) {
				print_help();
				exit(1);
			}
			query_timeout = std::stoi(splits[1]);

		} else if (StringUtil::StartsWith(arg, "--fetch_timeout=")) {
			auto splits = StringUtil::Split(arg, '=');
			if (splits.size() != 2) {
				print_help();
				exit(1);
			}
			fetch_timeout = std::stoi(splits[1]);

		} else {
			fprintf(stderr, "Error: unknown argument %s\n", arg.c_str());
			print_help();
			exit(1);
		}
	}

	unordered_map<string, RestClientState> client_state_map;
	std::mutex client_state_map_mutex;
	std::thread client_state_cleanup_thread(client_state_cleanup, &client_state_map, &client_state_map_mutex,
	                                        fetch_timeout);

	if (!logfile_name.empty()) {
		logfile.open(logfile_name, std::ios_base::app);
	}

	GrainDB graindb(dbfile.empty() ? nullptr : dbfile.c_str(), &config);

	svr.Get("/query", [&](const Request &req, Response &resp) {
		auto q = req.get_param_value("q");
		{
			std::lock_guard<std::mutex> guard(out_mutex);
			logfile << q << " ; -- DFgoEnx9UIRgHFsVYW8K" << std::endl
			        << std::flush; // using a terminator that will **never** occur in queries
		}

		json j;

		RestClientState state;
		state.con = make_unique<Connection>(graindb);
		state.con->EnableProfiling();
		state.touched = std::time(nullptr);
		bool is_active = true;

		std::thread interrupt_thread(sleep_thread, state.con.get(), &is_active, query_timeout);
		auto res = state.con->context->Query(q, true);

		is_active = false;
		interrupt_thread.join();

		state.res = move(res);

		if (state.res->success) {
			j = {{"query", q},
			     {"success", state.res->success},
			     {"column_count", state.res->types.size()},

			     {"statement_type", StatementTypeToString(state.res->statement_type)},
			     {"names", json(state.res->names)},
			     {"name_index_map", json::object()},
			     {"types", json::array()},
			     {"sql_types", json::array()},
			     {"data", json::array()}};

			for (auto &sql_type : state.res->sql_types) {
				j["sql_types"] += SQLTypeToString(sql_type);
			}
			for (auto &type : state.res->types) {
				j["types"] += TypeIdToString(type);
			}

			// make it easier to get col data by name
			size_t col_idx = 0;
			for (auto &name : state.res->names) {
				j["name_index_map"][name] = col_idx;
				col_idx++;
			}

			// only do this if query was successful
			string query_ref = random_string(10);
			j["ref"] = query_ref;
			auto chunk = state.res->Fetch();
			serialize_chunk(state.res.get(), chunk.get(), j);
			{
				std::lock_guard<std::mutex> guard(client_state_map_mutex);
				client_state_map[query_ref] = move(state);
			}

		} else {
			j = {{"query", q}, {"success", state.res->success}, {"error", state.res->error}};
		}

		serialize_json(req, resp, j);
	});

	svr.Get("/fetch", [&](const Request &req, Response &resp) {
		auto ref = req.get_param_value("ref");
		json j;
		RestClientState state;
		bool found_state = false;
		{
			std::lock_guard<std::mutex> guard(client_state_map_mutex);
			auto it = client_state_map.find(ref);
			if (it != client_state_map.end()) {
				state = move(it->second);
				client_state_map.erase(it);
				found_state = true;
			}
		}

		if (found_state) {
			bool is_active = true;
			std::thread interrupt_thread(sleep_thread, state.con.get(), &is_active, query_timeout);
			auto chunk = state.res->Fetch();
			is_active = false;
			interrupt_thread.join();

			j = {{"success", true}, {"ref", ref}, {"count", chunk->size()}, {"data", json::array()}};
			serialize_chunk(state.res.get(), chunk.get(), j);
			if (chunk->size() != 0) {
				std::lock_guard<std::mutex> guard(client_state_map_mutex);
				state.touched = std::time(nullptr);
				client_state_map[ref] = move(state);
			}
		} else {
			j = {{"success", false}, {"error", "Unable to find ref."}};
		}

		serialize_json(req, resp, j);
	});

	svr.Get("/close", [&](const Request &req, Response &resp) {
		auto ref = req.get_param_value("ref");
		Connection conn(graindb);
		json j;
		std::lock_guard<std::mutex> guard(client_state_map_mutex);
		if (client_state_map.find(ref) != client_state_map.end()) {
			client_state_map.erase(client_state_map.find(ref));
			j = {{"success", true}, {"ref", ref}};
		} else {
			j = {{"success", false}, {"error", "Unable to find ref."}};
		}

		serialize_json(req, resp, j);
	});

	std::cout << "🦆 serving " + dbfile + " on http://" + listen + ":" + std::to_string(port) + "\n";

	svr.listen(listen.c_str(), port);
	return 0;
}
