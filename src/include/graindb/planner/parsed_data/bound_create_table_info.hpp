//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/parsed_data/bound_create_table_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_data/create_table_info.hpp"
#include "graindb/planner/bound_constraint.hpp"
#include "graindb/planner/expression.hpp"
#include "graindb/storage/table/persistent_segment.hpp"
#include "graindb/planner/logical_operator.hpp"

namespace graindb {
class CatalogEntry;

struct BoundCreateTableInfo {
	BoundCreateTableInfo(unique_ptr<CreateInfo> base) : base(move(base)) {
	}

	//! The schema to create the table in
	SchemaCatalogEntry *schema;
	//! The base CreateInfo object
	unique_ptr<CreateInfo> base;
	//! The map of column names -> column index, used during binding
	unordered_map<string, column_t> name_map;
	//! List of constraints on the table
	vector<unique_ptr<Constraint>> constraints;
	//! List of bound constraints on the table
	vector<unique_ptr<BoundConstraint>> bound_constraints;
	//! Bound default values
	vector<unique_ptr<Expression>> bound_defaults;
	//! Dependents of the table (in e.g. default values)
	unordered_set<CatalogEntry *> dependencies;
	//! The existing table data on disk (if any)
	unique_ptr<vector<unique_ptr<PersistentSegment>>[]> data;
	//! CREATE TABLE from QUERY
	unique_ptr<LogicalOperator> query;

	CreateTableInfo &Base() {
		return (CreateTableInfo &)*base;
	}
};

} // namespace graindb
