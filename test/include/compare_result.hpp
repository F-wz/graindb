#pragma once

#include "graindb/common/string_util.hpp"
#include "graindb.hpp"

namespace graindb {

bool CHECK_COLUMN(QueryResult &result, size_t column_number, vector<graindb::Value> values);
bool CHECK_COLUMN(unique_ptr<graindb::QueryResult> &result, size_t column_number, vector<graindb::Value> values);
bool CHECK_COLUMN(unique_ptr<graindb::MaterializedQueryResult> &result, size_t column_number,
                  vector<graindb::Value> values);

string compare_csv(graindb::QueryResult &result, string csv, bool header = false);

bool parse_datachunk(string csv, DataChunk &result, vector<SQLType> sql_types, bool has_header);

//! Compares the result of a pipe-delimited CSV with the given DataChunk
//! Returns true if they are equal, and stores an error_message otherwise
bool compare_result(string csv, ChunkCollection &collection, vector<SQLType> sql_types, bool has_header,
                    string &error_message);

} // namespace graindb
