//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/common/printer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/constants.hpp"

namespace graindb {

//! Printer is a static class that allows printing to logs or stdout/stderr
class Printer {
public:
	//! Print the object to stderr
	static void Print(string str);
};
} // namespace graindb
