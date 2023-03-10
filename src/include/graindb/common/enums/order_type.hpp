//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/common/enums/order_type.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/constants.hpp"

namespace graindb {

enum class OrderType : uint8_t { INVALID = 0, ASCENDING = 1, DESCENDING = 2 };
}
