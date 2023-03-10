#include "graindb.h"
#include <stdio.h>

int main() {
	graindb_database db = NULL;
	graindb_connection con = NULL;
	graindb_result result;

	if (graindb_open(NULL, &db) == GrainDBError) {
		fprintf(stderr, "Failed to open database\n");
		goto cleanup;
	}
	if (graindb_connect(db, &con) == GrainDBError) {
		fprintf(stderr, "Failed to open connection\n");
		goto cleanup;
	}
	if (graindb_query(con, "CREATE TABLE integers(i INTEGER, j INTEGER);", NULL) == GrainDBError) {
		fprintf(stderr, "Failed to query database\n");
		goto cleanup;
	}
	if (graindb_query(con, "INSERT INTO integers VALUES (3, 4), (5, 6), (7, NULL);", NULL) == GrainDBError) {
		fprintf(stderr, "Failed to query database\n");
		goto cleanup;
	}
	if (graindb_query(con, "SELECT * FROM integers", &result) == GrainDBError) {
		fprintf(stderr, "Failed to query database\n");
		goto cleanup;
	}
	// print the names of the result
	for (size_t i = 0; i < result.column_count; i++) {
		printf("%s ", result.columns[i].name);
	}
	printf("\n");
	// print the data of the result
	for (size_t row_idx = 0; row_idx < result.row_count; row_idx++) {
		for (size_t col_idx = 0; col_idx < result.column_count; col_idx++) {
			char *val = graindb_value_varchar(&result, col_idx, row_idx);
			printf("%s ", val);
			free(val);
		}
		printf("\n");
	}
	// graindb_print_result(result);
cleanup:
	graindb_destroy_result(&result);
	graindb_disconnect(&con);
	graindb_close(&db);
}
