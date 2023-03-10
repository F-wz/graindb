//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/parsed_data/vacuum_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/parsed_data/parse_info.hpp"

namespace graindb {

struct VacuumInfo : public ParseInfo {
	//! analyze = 2
	int vaccum_type;
};

} // namespace graindb
