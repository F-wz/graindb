//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/execution/physical_plan_generator.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/common.hpp"
#include "graindb/common/types/chunk_collection.hpp"
#include "graindb/execution/physical_operator.hpp"
#include "graindb/planner/logical_operator.hpp"
#include "graindb/planner/logical_tokens.hpp"

namespace graindb {
class ClientContext;

//! The physical plan generator generates a physical execution plan from a
//! logical query plan
class PhysicalPlanGenerator {
public:
	PhysicalPlanGenerator(ClientContext &context) : context(context) {
	}

	unordered_set<CatalogEntry *> dependencies;
	//! Recursive CTEs require at least one ChunkScan, referencing the working_table.
	//! This data structure is used to establish it.
	unordered_map<idx_t, std::shared_ptr<ChunkCollection>> rec_ctes;

public:
	//! Creates a plan from the logical operator. This involves resolving column bindings and generating physical
	//! operator nodes.
	unique_ptr<PhysicalOperator> CreatePlan(unique_ptr<LogicalOperator> logical);

protected:
	unique_ptr<PhysicalOperator> CreatePlan(LogicalOperator &op);

	unique_ptr<PhysicalOperator> CreatePlan(LogicalAggregate &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalAnyJoin &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalChunkGet &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalComparisonJoin &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalCreate &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalCreateTable &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalCreateIndex &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalCreateRAI &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalCrossProduct &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalDelete &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalDelimGet &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalDelimJoin &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalDistinct &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalEmptyResult &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalExpressionGet &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalFilter &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalGet &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalLimit &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalIndexScan &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalOrder &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalTopN &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalProjection &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalInsert &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalCopyFromFile &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalCopyToFile &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalExplain &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalSetOperation &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalUpdate &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalTableFunction &expr);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalPrepare &expr);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalWindow &expr);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalExecute &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalSimple &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalUnnest &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalRecursiveCTE &op);
	unique_ptr<PhysicalOperator> CreatePlan(LogicalCTERef &op);

	unique_ptr<PhysicalOperator> CreateDistinct(unique_ptr<PhysicalOperator> child);
	unique_ptr<PhysicalOperator> CreateDistinctOn(unique_ptr<PhysicalOperator> child,
	                                              vector<unique_ptr<Expression>> distinct_targets);

private:
	ClientContext &context;
};
} // namespace graindb
