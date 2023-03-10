#pragma once

#include "graindb/common/constants.hpp"
#include "graindb/common/types/chunk_collection.hpp"
#include "graindb/execution/operator/join/physical_comparison_join.hpp"
#include "graindb/execution/physical_operator.hpp"
#include "graindb/execution/sip_hashtable.hpp"
#include "graindb/planner/operator/logical_join.hpp"
#include "graindb/storage/rai.hpp"

namespace graindb {

class PhysicalSIPJoin : public PhysicalComparisonJoin {
public:
	PhysicalSIPJoin(ClientContext &context, LogicalOperator &op, unique_ptr<PhysicalOperator> left,
	                unique_ptr<PhysicalOperator> right, vector<JoinCondition> cond, JoinType join_type,
	                vector<idx_t> &left_projection_map, vector<idx_t> &right_projection_map);

private:
	unique_ptr<SIPHashTable> hash_table;
	vector<idx_t> right_projection_map;

public:
	void GetChunkInternal(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_,
	                      SelectionVector *sel = nullptr, Vector *rid_vector = nullptr,
	                      DataChunk *rai_chunk = nullptr) override;
	unique_ptr<PhysicalOperatorState> GetOperatorState() override;
	string ExtraRenderInformation() const override;

private:
	void InitializeAList();
//	void InitializeBitMaskFilter();
	void PassBitMaskFilter();
	void ProbeHashTable(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_);
	void PerformSHJoin(ClientContext &context, DataChunk &chunk, PhysicalOperatorState *state_);
};

} // namespace graindb
