//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/main/extension.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/common.hpp"

namespace graindb {
class GrainDB;

//! The Extension class is the base class used to define extensions
class Extension {
public:
	virtual void Load(GrainDB &db) = 0;
};

} // namespace graindb
