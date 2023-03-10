//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/parsed_data/pragma_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_data/parse_info.hpp"
#include "graindb/common/types/value.hpp"

namespace graindb {

enum class PragmaType : uint8_t { NOTHING, ASSIGNMENT, CALL };

struct PragmaInfo : public ParseInfo {
	//! Name of the PRAGMA statement
	string name;
	//! Type of pragma statement
	PragmaType pragma_type;
	//! Parameter list (if any)
	vector<Value> parameters;
};

} // namespace graindb
