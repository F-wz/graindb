//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/operator/scan/physical_expression_scan.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/types/chunk_collection.hpp"
#include "graindb/execution/physical_operator.hpp"

namespace graindb {

//! The PhysicalExpressionScan scans a set of expressions
class PhysicalExpressionScan : public PhysicalOperator {
public:
	PhysicalExpressionScan(vector<TypeId> types, vector<vector<unique_ptr<Expression>>> expressions)
	    : PhysicalOperator(PhysicalOperatorType::EXPRESSION_SCAN, types), expressions(move(expressions)) {
	}

	//! The set of expressions to scan
	vector<vector<unique_ptr<Expression>>> expressions;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
	unique_ptr<PhysicalOperatorState> GetOperatorState() override;
};

} // namespace graindb
