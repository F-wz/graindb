//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/parsed_data/create_index_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_data/create_info.hpp"
#include "graindb/common/enums/index_type.hpp"
#include "graindb/parser/tableref/basetableref.hpp"
#include "graindb/parser/parsed_expression.hpp"

namespace graindb {

struct CreateIndexInfo : public CreateInfo {
	CreateIndexInfo() : CreateInfo(CatalogType::INDEX) {
	}

	//! Index Type (e.g., B+-tree, Skip-List, ...)
	IndexType index_type;
	//! Name of the Index
	string index_name;
	//! If it is an unique index
	bool unique = false;
	//! The table to create the index on
	unique_ptr<BaseTableRef> table;
	//! Set of expressions to index by
	vector<unique_ptr<ParsedExpression>> expressions;
};

} // namespace graindb
