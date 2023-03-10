//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/common/serializer/buffered_deserializer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/serializer/buffered_serializer.hpp"
#include "graindb/common/serializer.hpp"

namespace graindb {

class BufferedDeserializer : public Deserializer {
public:
	BufferedDeserializer(data_ptr_t ptr, idx_t data_size);
	BufferedDeserializer(BufferedSerializer &serializer);

	data_ptr_t ptr;
	data_ptr_t endptr;

public:
	void ReadData(data_ptr_t buffer, uint64_t read_size) override;
};

} // namespace graindb
