//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/parsed_data/create_edge_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/enums/rai_direction.hpp"
#include "graindb/parser/parsed_data/create_info.hpp"
#include "graindb/parser/tableref/basetableref.hpp"

namespace graindb {

struct CreateRAIInfo : public CreateInfo {
	CreateRAIInfo() : CreateInfo(CatalogType::RAI), name(""), table(nullptr), direction(RAIDirection::DIRECTED) {
	}

	string name;
	unique_ptr<TableRef> table;
	RAIDirection direction;
	vector<unique_ptr<TableRef>> referenced_tables;
	vector<unique_ptr<ParsedExpression>> columns;
	vector<unique_ptr<ParsedExpression>> references;
};
} // namespace graindb
