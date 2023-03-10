//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/common/enums/profiler_format.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/constants.hpp"

namespace graindb {

enum class ProfilerPrintFormat : uint8_t { NONE, QUERY_TREE, JSON };

} // namespace graindb
