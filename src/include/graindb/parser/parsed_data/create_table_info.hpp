//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/parsed_data/create_table_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_data/create_info.hpp"
#include "graindb/common/unordered_set.hpp"
#include "graindb/parser/column_definition.hpp"
#include "graindb/parser/constraint.hpp"
#include "graindb/parser/statement/select_statement.hpp"

namespace graindb {

struct CreateTableInfo : public CreateInfo {
	CreateTableInfo() : CreateInfo(CatalogType::TABLE, INVALID_SCHEMA) {
	}
	CreateTableInfo(string schema, string name) : CreateInfo(CatalogType::TABLE, schema), table(name) {
	}

	//! Table name to insert to
	string table;
	//! List of columns of the table
	vector<ColumnDefinition> columns;
	//! List of constraints on the table
	vector<unique_ptr<Constraint>> constraints;
	//! CREATE TABLE from QUERY
	unique_ptr<SelectStatement> query;
};

} // namespace graindb
