//===----------------------------------------------------------------------===//
//
//                         GrainDB
//
// graindb.h
//
// Author: Mark Raasveldt
//
//===----------------------------------------------------------------------===//

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t idx_t;

typedef enum GRAINDB_TYPE {
	GRAINDB_TYPE_INVALID = 0,
	// bool
	GRAINDB_TYPE_BOOLEAN,
	// int8_t
	GRAINDB_TYPE_TINYINT,
	// int16_t
	GRAINDB_TYPE_SMALLINT,
	// int32_t
	GRAINDB_TYPE_INTEGER,
	// int64_t
	GRAINDB_TYPE_BIGINT,
	// float
	GRAINDB_TYPE_FLOAT,
	// double
	GRAINDB_TYPE_DOUBLE,
	// graindb_timestamp
	GRAINDB_TYPE_TIMESTAMP,
	// graindb_date
	GRAINDB_TYPE_DATE,
	// graindb_time
	GRAINDB_TYPE_TIME,
	// const char*
	GRAINDB_TYPE_VARCHAR
} graindb_type;

typedef struct {
	int32_t year;
	int8_t month;
	int8_t day;
} graindb_date;

typedef struct {
	int8_t hour;
	int8_t min;
	int8_t sec;
	int16_t msec;
} graindb_time;

typedef struct {
	graindb_date date;
	graindb_time time;
} graindb_timestamp;

typedef struct {
	void *data;
	bool *nullmask;
	graindb_type type;
	char *name;
} graindb_column;

typedef struct {
	idx_t column_count;
	idx_t row_count;
	graindb_column *columns;
	char *error_message;
} graindb_result;

// typedef struct {
// 	void *data;
// 	bool *nullmask;
// } graindb_column_data;

// typedef struct {
// 	int column_count;
// 	int count;
// 	graindb_column_data *columns;
// } graindb_chunk;

typedef void *graindb_database;
typedef void *graindb_connection;
typedef void *graindb_prepared_statement;

typedef enum { GrainDBSuccess = 0, GrainDBError = 1 } graindb_state;

//! Opens a database file at the given path (nullptr for in-memory). Returns GrainDBSuccess on success, or GrainDBError on
//! failure. [OUT: database]
graindb_state graindb_open(const char *path, graindb_database *out_database);
//! Closes the database.
void graindb_close(graindb_database *database);

//! Creates a connection to the specified database. [OUT: connection]
graindb_state graindb_connect(graindb_database database, graindb_connection *out_connection);
//! Closes the specified connection handle
void graindb_disconnect(graindb_connection *connection);

//! Executes the specified SQL query in the specified connection handle. [OUT: result descriptor]
graindb_state graindb_query(graindb_connection connection, const char *query, graindb_result *out_result);
//! Destroys the specified result
void graindb_destroy_result(graindb_result *result);

// SAFE fetch functions
// These functions will perform conversions if necessary. On failure (e.g. if conversion cannot be performed) a special
// value is returned.

//! Converts the specified value to a bool. Returns false on failure or NULL.
bool graindb_value_boolean(graindb_result *result, idx_t col, idx_t row);
//! Converts the specified value to an int8_t. Returns 0 on failure or NULL.
int8_t graindb_value_int8(graindb_result *result, idx_t col, idx_t row);
//! Converts the specified value to an int16_t. Returns 0 on failure or NULL.
int16_t graindb_value_int16(graindb_result *result, idx_t col, idx_t row);
//! Converts the specified value to an int64_t. Returns 0 on failure or NULL.
int32_t graindb_value_int32(graindb_result *result, idx_t col, idx_t row);
//! Converts the specified value to an int64_t. Returns 0 on failure or NULL.
int64_t graindb_value_int64(graindb_result *result, idx_t col, idx_t row);
//! Converts the specified value to a float. Returns 0.0 on failure or NULL.
float graindb_value_float(graindb_result *result, idx_t col, idx_t row);
//! Converts the specified value to a double. Returns 0.0 on failure or NULL.
double graindb_value_double(graindb_result *result, idx_t col, idx_t row);
//! Converts the specified value to a string. Returns nullptr on failure or NULL. The result must be freed with free.
char *graindb_value_varchar(graindb_result *result, idx_t col, idx_t row);

// Prepared Statements

//! prepares the specified SQL query in the specified connection handle. [OUT: prepared statement descriptor]
graindb_state graindb_prepare(graindb_connection connection, const char *query,
                            graindb_prepared_statement *out_prepared_statement);

graindb_state graindb_nparams(graindb_prepared_statement prepared_statement, idx_t *nparams_out);

//! binds parameters to prepared statement
graindb_state graindb_bind_boolean(graindb_prepared_statement prepared_statement, idx_t param_idx, bool val);
graindb_state graindb_bind_int8(graindb_prepared_statement prepared_statement, idx_t param_idx, int8_t val);
graindb_state graindb_bind_int16(graindb_prepared_statement prepared_statement, idx_t param_idx, int16_t val);
graindb_state graindb_bind_int32(graindb_prepared_statement prepared_statement, idx_t param_idx, int32_t val);
graindb_state graindb_bind_int64(graindb_prepared_statement prepared_statement, idx_t param_idx, int64_t val);
graindb_state graindb_bind_float(graindb_prepared_statement prepared_statement, idx_t param_idx, float val);
graindb_state graindb_bind_double(graindb_prepared_statement prepared_statement, idx_t param_idx, double val);
graindb_state graindb_bind_varchar(graindb_prepared_statement prepared_statement, idx_t param_idx, const char *val);
graindb_state graindb_bind_null(graindb_prepared_statement prepared_statement, idx_t param_idx);

//! Executes the prepared statements with currently bound parameters
graindb_state graindb_execute_prepared(graindb_prepared_statement prepared_statement, graindb_result *out_result);

//! Destroys the specified prepared statement descriptor
void graindb_destroy_prepare(graindb_prepared_statement *prepared_statement);

#ifdef __cplusplus
};
#endif
