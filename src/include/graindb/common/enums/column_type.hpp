//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/common/enums/column_type.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/constants.hpp"

namespace graindb {

enum class ColumnType : uint8_t { COMMON = 0, FROM = 1, TO = 2, LABEL = 3 };
}
