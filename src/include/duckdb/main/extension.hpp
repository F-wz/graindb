//===----------------------------------------------------------------------===//
//                         GrainDB
//
// duckdb/main/extension.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/common.hpp"

namespace duckdb {
class GrainDB;

//! The Extension class is the base class used to define extensions
class Extension {
public:
	virtual void Load(GrainDB &db) = 0;
};

} // namespace duckdb
