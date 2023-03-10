//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/parsed_data/drop_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_data/parse_info.hpp"
#include "graindb/common/enums/catalog_type.hpp"

namespace graindb {

struct DropInfo : public ParseInfo {
	DropInfo() : schema(INVALID_SCHEMA), if_exists(false), cascade(false) {
	}

	//! The catalog type to drop
	CatalogType type;
	//! Schema name to drop from, if any
	string schema;
	//! Element name to drop
	string name;
	//! Ignore if the entry does not exist instead of failing
	bool if_exists = false;
	//! Cascade drop (drop all dependents instead of throwing an error if there
	//! are any)
	bool cascade = false;
};

} // namespace graindb
