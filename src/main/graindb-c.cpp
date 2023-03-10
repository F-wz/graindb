#include "graindb/common/types/date.hpp"
#include "graindb/common/types/time.hpp"
#include "graindb/common/types/timestamp.hpp"
#include "graindb/common/vector_operations/vector_operations.hpp"
#include "graindb.h"
#include "graindb.hpp"

#include <cstring>

#ifdef _WIN32
#define strdup _strdup
#endif

using namespace graindb;

static SQLType ConvertCTypeToCPP(graindb_type type);
static graindb_type ConvertCPPTypeToC(SQLType type);
static idx_t GetCTypeSize(graindb_type type);

struct DatabaseData {
	DatabaseData() : database(nullptr) {
	}
	~DatabaseData() {
		if (database) {
			delete database;
		}
	}

	GrainDB *database;
};

graindb_state graindb_open(const char *path, graindb_database *out) {
	auto wrapper = new DatabaseData();
	try {
		wrapper->database = new GrainDB(path);
	} catch (...) {
		delete wrapper;
		return GrainDBError;
	}
	*out = (graindb_database)wrapper;
	return GrainDBSuccess;
}

void graindb_close(graindb_database *database) {
	if (*database) {
		auto wrapper = (DatabaseData *)*database;
		delete wrapper;
		*database = nullptr;
	}
}

graindb_state graindb_connect(graindb_database database, graindb_connection *out) {
	auto wrapper = (DatabaseData *)database;
	Connection *connection;
	try {
		connection = new Connection(*wrapper->database);
	} catch (...) {
		return GrainDBError;
	}
	*out = (graindb_connection)connection;
	return GrainDBSuccess;
}

void graindb_disconnect(graindb_connection *connection) {
	if (*connection) {
		Connection *conn = (Connection *)*connection;
		delete conn;
		*connection = nullptr;
	}
}

template <class T> void WriteData(graindb_result *out, ChunkCollection &source, idx_t col) {
	idx_t row = 0;
	auto target = (T *)out->columns[col].data;
	for (auto &chunk : source.chunks) {
		auto source = FlatVector::GetData<T>(chunk->data[col]);
		for (idx_t k = 0; k < chunk->size(); k++) {
			target[row++] = source[k];
		}
	}
}

static graindb_state graindb_translate_result(MaterializedQueryResult *result, graindb_result *out) {
	assert(result);
	if (!out) {
		// no result to write to, only return the status
		return result->success ? GrainDBSuccess : GrainDBError;
	}
	out->error_message = NULL;
	if (!result->success) {
		// write the error message
		out->error_message = strdup(result->error.c_str());
		return GrainDBError;
	}
	// copy the data
	// first write the meta data
	out->column_count = result->types.size();
	out->row_count = result->collection.count;
	out->columns = (graindb_column *)malloc(sizeof(graindb_column) * out->column_count);
	if (!out->columns) {
		return GrainDBError;
	}
	// zero initialize the columns (so we can cleanly delete it in case a malloc fails)
	memset(out->columns, 0, sizeof(graindb_column) * out->column_count);
	for (idx_t i = 0; i < out->column_count; i++) {
		out->columns[i].type = ConvertCPPTypeToC(result->sql_types[i]);
		out->columns[i].name = strdup(result->names[i].c_str());
		out->columns[i].nullmask = (bool *)malloc(sizeof(bool) * out->row_count);
		out->columns[i].data = malloc(GetCTypeSize(out->columns[i].type) * out->row_count);
		if (!out->columns[i].nullmask || !out->columns[i].name || !out->columns[i].data) {
			// malloc failure
			return GrainDBError;
		}
		// memset data to 0 for VARCHAR columns for safe deletion later
		if (result->types[i] == TypeId::VARCHAR) {
			memset(out->columns[i].data, 0, GetCTypeSize(out->columns[i].type) * out->row_count);
		}
	}
	// now write the data
	for (idx_t col = 0; col < out->column_count; col++) {
		// first set the nullmask
		idx_t row = 0;
		for (auto &chunk : result->collection.chunks) {
			for (idx_t k = 0; k < chunk->size(); k++) {
				out->columns[col].nullmask[row++] = FlatVector::IsNull(chunk->data[col], k);
			}
		}
		// then write the data
		switch (result->sql_types[col].id) {
		case SQLTypeId::BOOLEAN:
			WriteData<bool>(out, result->collection, col);
			break;
		case SQLTypeId::TINYINT:
			WriteData<int8_t>(out, result->collection, col);
			break;
		case SQLTypeId::SMALLINT:
			WriteData<int16_t>(out, result->collection, col);
			break;
		case SQLTypeId::INTEGER:
			WriteData<int32_t>(out, result->collection, col);
			break;
		case SQLTypeId::BIGINT:
			WriteData<int64_t>(out, result->collection, col);
			break;
		case SQLTypeId::FLOAT:
			WriteData<float>(out, result->collection, col);
			break;
		case SQLTypeId::DECIMAL:
		case SQLTypeId::DOUBLE:
			WriteData<double>(out, result->collection, col);
			break;
		case SQLTypeId::VARCHAR: {
			idx_t row = 0;
			auto target = (const char **)out->columns[col].data;
			for (auto &chunk : result->collection.chunks) {
				auto source = FlatVector::GetData<string_t>(chunk->data[col]);
				for (idx_t k = 0; k < chunk->size(); k++) {
					if (!FlatVector::IsNull(chunk->data[col], k)) {
						target[row] = strdup(source[k].GetData());
					}
					row++;
				}
			}
			break;
		}
		case SQLTypeId::DATE: {
			idx_t row = 0;
			auto target = (graindb_date *)out->columns[col].data;
			for (auto &chunk : result->collection.chunks) {
				auto source = FlatVector::GetData<date_t>(chunk->data[col]);
				for (idx_t k = 0; k < chunk->size(); k++) {
					if (!FlatVector::IsNull(chunk->data[col], k)) {
						int32_t year, month, day;
						Date::Convert(source[k], year, month, day);
						target[row].year = year;
						target[row].month = month;
						target[row].day = day;
					}
					row++;
				}
			}
			break;
		}
		case SQLTypeId::TIME: {
			idx_t row = 0;
			auto target = (graindb_time *)out->columns[col].data;
			for (auto &chunk : result->collection.chunks) {
				auto source = FlatVector::GetData<dtime_t>(chunk->data[col]);
				for (idx_t k = 0; k < chunk->size(); k++) {
					if (!FlatVector::IsNull(chunk->data[col], k)) {
						int32_t hour, min, sec, msec;
						Time::Convert(source[k], hour, min, sec, msec);
						target[row].hour = hour;
						target[row].min = min;
						target[row].sec = sec;
						target[row].msec = msec;
					}
					row++;
				}
			}
			break;
		}
		case SQLTypeId::TIMESTAMP: {
			idx_t row = 0;
			auto target = (graindb_timestamp *)out->columns[col].data;
			for (auto &chunk : result->collection.chunks) {
				auto source = FlatVector::GetData<timestamp_t>(chunk->data[col]);
				for (idx_t k = 0; k < chunk->size(); k++) {
					if (!FlatVector::IsNull(chunk->data[col], k)) {
						date_t date;
						dtime_t time;
						Timestamp::Convert(source[k], date, time);

						int32_t year, month, day;
						Date::Convert(date, year, month, day);

						int32_t hour, min, sec, msec;
						Time::Convert(time, hour, min, sec, msec);

						target[row].date.year = year;
						target[row].date.month = month;
						target[row].date.day = day;
						target[row].time.hour = hour;
						target[row].time.min = min;
						target[row].time.sec = sec;
						target[row].time.msec = msec;
					}
					row++;
				}
			}
			break;
		}
		default:
			// unsupported type for C API
			assert(0);
			return GrainDBError;
		}
	}
	return GrainDBSuccess;
}

graindb_state graindb_query(graindb_connection connection, const char *query, graindb_result *out) {
	Connection *conn = (Connection *)connection;
	auto result = conn->Query(query);
	return graindb_translate_result(result.get(), out);
}

static void graindb_destroy_column(graindb_column column, idx_t count) {
	if (column.data) {
		if (column.type == GRAINDB_TYPE_VARCHAR) {
			// varchar, delete individual strings
			auto data = (char **)column.data;
			for (idx_t i = 0; i < count; i++) {
				if (data[i]) {
					free(data[i]);
				}
			}
		}
		free(column.data);
	}
	if (column.nullmask) {
		free(column.nullmask);
	}
	if (column.name) {
		free(column.name);
	}
}

void graindb_destroy_result(graindb_result *result) {
	if (result->error_message) {
		free(result->error_message);
	}
	if (result->columns) {
		for (idx_t i = 0; i < result->column_count; i++) {
			graindb_destroy_column(result->columns[i], result->row_count);
		}
		free(result->columns);
	}
	memset(result, 0, sizeof(graindb_result));
}

struct PreparedStatementWrapper {
	PreparedStatementWrapper() : statement(nullptr) {
	}
	~PreparedStatementWrapper() {
	}
	unique_ptr<PreparedStatement> statement;
	vector<Value> values;
};

graindb_state graindb_prepare(graindb_connection connection, const char *query,
                            graindb_prepared_statement *out_prepared_statement) {
	if (!connection || !query) {
		return GrainDBError;
	}
	auto wrapper = new PreparedStatementWrapper();
	Connection *conn = (Connection *)connection;
	wrapper->statement = conn->Prepare(query);
	*out_prepared_statement = (graindb_prepared_statement)wrapper;
	return wrapper->statement->success ? GrainDBSuccess : GrainDBError;
}

graindb_state graindb_nparams(graindb_prepared_statement prepared_statement, idx_t *nparams_out) {
	auto wrapper = (PreparedStatementWrapper *)prepared_statement;
	if (!wrapper || !wrapper->statement || !wrapper->statement->success || wrapper->statement->is_invalidated) {
		return GrainDBError;
	}
	*nparams_out = wrapper->statement->n_param;
	return GrainDBSuccess;
}

static graindb_state graindb_bind_value(graindb_prepared_statement prepared_statement, idx_t param_idx, Value val) {
	auto wrapper = (PreparedStatementWrapper *)prepared_statement;
	if (!wrapper || !wrapper->statement || !wrapper->statement->success || wrapper->statement->is_invalidated) {
		return GrainDBError;
	}
	if (param_idx > wrapper->statement->n_param) {
		return GrainDBError;
	}
	if (param_idx > wrapper->values.size()) {
		wrapper->values.resize(param_idx);
	}
	wrapper->values[param_idx - 1] = val;
	return GrainDBSuccess;
}

graindb_state graindb_bind_boolean(graindb_prepared_statement prepared_statement, idx_t param_idx, bool val) {
	return graindb_bind_value(prepared_statement, param_idx, Value::BOOLEAN(val));
}

graindb_state graindb_bind_int8(graindb_prepared_statement prepared_statement, idx_t param_idx, int8_t val) {
	return graindb_bind_value(prepared_statement, param_idx, Value::TINYINT(val));
}

graindb_state graindb_bind_int16(graindb_prepared_statement prepared_statement, idx_t param_idx, int16_t val) {
	return graindb_bind_value(prepared_statement, param_idx, Value::SMALLINT(val));
}

graindb_state graindb_bind_int32(graindb_prepared_statement prepared_statement, idx_t param_idx, int32_t val) {
	return graindb_bind_value(prepared_statement, param_idx, Value::INTEGER(val));
}

graindb_state graindb_bind_int64(graindb_prepared_statement prepared_statement, idx_t param_idx, int64_t val) {
	return graindb_bind_value(prepared_statement, param_idx, Value::BIGINT(val));
}

graindb_state graindb_bind_float(graindb_prepared_statement prepared_statement, idx_t param_idx, float val) {
	return graindb_bind_value(prepared_statement, param_idx, Value(val));
}

graindb_state graindb_bind_double(graindb_prepared_statement prepared_statement, idx_t param_idx, double val) {
	return graindb_bind_value(prepared_statement, param_idx, Value(val));
}

graindb_state graindb_bind_varchar(graindb_prepared_statement prepared_statement, idx_t param_idx, const char *val) {
	return graindb_bind_value(prepared_statement, param_idx, Value(val));
}

graindb_state graindb_bind_null(graindb_prepared_statement prepared_statement, idx_t param_idx) {
	return graindb_bind_value(prepared_statement, param_idx, Value());
}

graindb_state graindb_execute_prepared(graindb_prepared_statement prepared_statement, graindb_result *out_result) {
	auto wrapper = (PreparedStatementWrapper *)prepared_statement;
	if (!wrapper || !wrapper->statement || !wrapper->statement->success || wrapper->statement->is_invalidated) {
		return GrainDBError;
	}
	auto result = wrapper->statement->Execute(wrapper->values, false);
	assert(result->type == QueryResultType::MATERIALIZED_RESULT);
	auto mat_res = (MaterializedQueryResult *)result.get();
	return graindb_translate_result(mat_res, out_result);
}

void graindb_destroy_prepare(graindb_prepared_statement *prepared_statement) {
	if (!prepared_statement) {
		return;
	}
	auto wrapper = (PreparedStatementWrapper *)*prepared_statement;
	if (wrapper) {
		delete wrapper;
	}
	*prepared_statement = nullptr;
}

graindb_type ConvertCPPTypeToC(SQLType sql_type) {
	switch (sql_type.id) {
	case SQLTypeId::BOOLEAN:
		return GRAINDB_TYPE_BOOLEAN;
	case SQLTypeId::TINYINT:
		return GRAINDB_TYPE_TINYINT;
	case SQLTypeId::SMALLINT:
		return GRAINDB_TYPE_SMALLINT;
	case SQLTypeId::INTEGER:
		return GRAINDB_TYPE_INTEGER;
	case SQLTypeId::BIGINT:
		return GRAINDB_TYPE_BIGINT;
	case SQLTypeId::FLOAT:
		return GRAINDB_TYPE_FLOAT;
	case SQLTypeId::DECIMAL:
	case SQLTypeId::DOUBLE:
		return GRAINDB_TYPE_DOUBLE;
	case SQLTypeId::TIMESTAMP:
		return GRAINDB_TYPE_TIMESTAMP;
	case SQLTypeId::DATE:
		return GRAINDB_TYPE_DATE;
	case SQLTypeId::TIME:
		return GRAINDB_TYPE_TIME;
	case SQLTypeId::VARCHAR:
		return GRAINDB_TYPE_VARCHAR;
	default:
		return GRAINDB_TYPE_INVALID;
	}
}

SQLType ConvertCTypeToCPP(graindb_type type) {
	switch (type) {
	case GRAINDB_TYPE_BOOLEAN:
		return SQLType(SQLTypeId::BOOLEAN);
	case GRAINDB_TYPE_TINYINT:
		return SQLType::TINYINT;
	case GRAINDB_TYPE_SMALLINT:
		return SQLType::SMALLINT;
	case GRAINDB_TYPE_INTEGER:
		return SQLType::INTEGER;
	case GRAINDB_TYPE_BIGINT:
		return SQLType::BIGINT;
	case GRAINDB_TYPE_FLOAT:
		return SQLType::FLOAT;
	case GRAINDB_TYPE_DOUBLE:
		return SQLType::DOUBLE;
	case GRAINDB_TYPE_TIMESTAMP:
		return SQLType::TIMESTAMP;
	case GRAINDB_TYPE_DATE:
		return SQLType::DATE;
	case GRAINDB_TYPE_TIME:
		return SQLType::TIME;
	case GRAINDB_TYPE_VARCHAR:
		return SQLType::VARCHAR;
	default:
		return SQLType(SQLTypeId::INVALID);
	}
}

idx_t GetCTypeSize(graindb_type type) {
	switch (type) {
	case GRAINDB_TYPE_BOOLEAN:
		return sizeof(bool);
	case GRAINDB_TYPE_TINYINT:
		return sizeof(int8_t);
	case GRAINDB_TYPE_SMALLINT:
		return sizeof(int16_t);
	case GRAINDB_TYPE_INTEGER:
		return sizeof(int32_t);
	case GRAINDB_TYPE_BIGINT:
		return sizeof(int64_t);
	case GRAINDB_TYPE_FLOAT:
		return sizeof(float);
	case GRAINDB_TYPE_DOUBLE:
		return sizeof(double);
	case GRAINDB_TYPE_DATE:
		return sizeof(graindb_date);
	case GRAINDB_TYPE_TIME:
		return sizeof(graindb_time);
	case GRAINDB_TYPE_TIMESTAMP:
		return sizeof(graindb_timestamp);
	case GRAINDB_TYPE_VARCHAR:
		return sizeof(const char *);
	default:
		// unsupported type
		assert(0);
		return sizeof(const char *);
	}
}

template <class T> T UnsafeFetch(graindb_result *result, idx_t col, idx_t row) {
	assert(row < result->row_count);
	return ((T *)result->columns[col].data)[row];
}

static Value GetCValue(graindb_result *result, idx_t col, idx_t row) {
	if (col >= result->column_count) {
		return Value();
	}
	if (row >= result->row_count) {
		return Value();
	}
	if (result->columns[col].nullmask[row]) {
		return Value();
	}
	switch (result->columns[col].type) {
	case GRAINDB_TYPE_BOOLEAN:
		return Value::BOOLEAN(UnsafeFetch<bool>(result, col, row));
	case GRAINDB_TYPE_TINYINT:
		return Value::TINYINT(UnsafeFetch<int8_t>(result, col, row));
	case GRAINDB_TYPE_SMALLINT:
		return Value::SMALLINT(UnsafeFetch<int16_t>(result, col, row));
	case GRAINDB_TYPE_INTEGER:
		return Value::INTEGER(UnsafeFetch<int32_t>(result, col, row));
	case GRAINDB_TYPE_BIGINT:
		return Value::BIGINT(UnsafeFetch<int64_t>(result, col, row));
	case GRAINDB_TYPE_FLOAT:
		return Value(UnsafeFetch<float>(result, col, row));
	case GRAINDB_TYPE_DOUBLE:
		return Value(UnsafeFetch<double>(result, col, row));
	case GRAINDB_TYPE_DATE: {
		auto date = UnsafeFetch<graindb_date>(result, col, row);
		return Value::DATE(date.year, date.month, date.day);
	}
	case GRAINDB_TYPE_TIME: {
		auto time = UnsafeFetch<graindb_time>(result, col, row);
		return Value::TIME(time.hour, time.min, time.sec, time.msec);
	}
	case GRAINDB_TYPE_TIMESTAMP: {
		auto timestamp = UnsafeFetch<graindb_timestamp>(result, col, row);
		return Value::TIMESTAMP(timestamp.date.year, timestamp.date.month, timestamp.date.day, timestamp.time.hour,
		                        timestamp.time.min, timestamp.time.sec, timestamp.time.msec);
	}
	case GRAINDB_TYPE_VARCHAR:
		return Value(string(UnsafeFetch<const char *>(result, col, row)));
	default:
		// invalid type for C to C++ conversion
		assert(0);
		return Value();
	}
}

bool graindb_value_boolean(graindb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return false;
	} else {
		return val.CastAs(TypeId::BOOL).value_.boolean;
	}
}

int8_t graindb_value_int8(graindb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return 0;
	} else {
		return val.CastAs(TypeId::INT8).value_.tinyint;
	}
}

int16_t graindb_value_int16(graindb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return 0;
	} else {
		return val.CastAs(TypeId::INT16).value_.smallint;
	}
}

int32_t graindb_value_int32(graindb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return 0;
	} else {
		return val.CastAs(TypeId::INT32).value_.integer;
	}
}

int64_t graindb_value_int64(graindb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return 0;
	} else {
		return val.CastAs(TypeId::INT64).value_.bigint;
	}
}

float graindb_value_float(graindb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return 0.0;
	} else {
		return val.CastAs(TypeId::FLOAT).value_.float_;
	}
}

double graindb_value_double(graindb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	if (val.is_null) {
		return 0.0;
	} else {
		return val.CastAs(TypeId::DOUBLE).value_.double_;
	}
}

char *graindb_value_varchar(graindb_result *result, idx_t col, idx_t row) {
	Value val = GetCValue(result, col, row);
	return strdup(val.ToString(ConvertCTypeToCPP(result->columns[col].type)).c_str());
}
