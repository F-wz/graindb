//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/common/enums/set_operation_type.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/constants.hpp"

namespace graindb {

enum class SetOperationType : uint8_t { NONE = 0, UNION = 1, EXCEPT = 2, INTERSECT = 3 };
}
