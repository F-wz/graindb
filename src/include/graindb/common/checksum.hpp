//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/common/checksum.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/constants.hpp"

namespace graindb {

//! Compute a checksum over a buffer of size size
uint64_t Checksum(uint8_t *buffer, size_t size);

} // namespace graindb
