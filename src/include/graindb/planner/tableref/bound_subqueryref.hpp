//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/planner/tableref/bound_subqueryref.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/planner/binder.hpp"
#include "graindb/planner/bound_query_node.hpp"
#include "graindb/planner/bound_tableref.hpp"

namespace graindb {

//! Represents a cross product
class BoundSubqueryRef : public BoundTableRef {
public:
	BoundSubqueryRef(unique_ptr<Binder> binder, unique_ptr<BoundQueryNode> subquery)
	    : BoundTableRef(TableReferenceType::SUBQUERY), binder(move(binder)), subquery(move(subquery)) {
	}

	//! The binder used to bind the subquery
	unique_ptr<Binder> binder;
	//! The bound subquery node
	unique_ptr<BoundQueryNode> subquery;
};
} // namespace graindb
