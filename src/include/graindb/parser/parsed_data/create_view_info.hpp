//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/parsed_data/create_view_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_data/create_info.hpp"
#include "graindb/parser/query_node.hpp"

namespace graindb {

struct CreateViewInfo : public CreateInfo {
	CreateViewInfo() : CreateInfo(CatalogType::VIEW) {
	}
	CreateViewInfo(string schema, string view_name) : CreateInfo(CatalogType::VIEW, schema), view_name(view_name) {
	}

	//! Table name to insert to
	string view_name;
	//! Aliases of the view
	vector<string> aliases;
	//! Return types
	vector<SQLType> types;
	//! The QueryNode of the view
	unique_ptr<QueryNode> query;
};

} // namespace graindb
