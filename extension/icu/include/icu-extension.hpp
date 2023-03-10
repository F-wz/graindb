//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/main/extension.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/main/extension.hpp"

namespace graindb {

class ICUExtension : public Extension {
public:
	void Load(GrainDB &db) override;
};


} // namespace graindb
