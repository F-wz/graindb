//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/common/enums/relation_type.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/common/constants.hpp"

namespace graindb {

//===--------------------------------------------------------------------===//
// Catalog Types
//===--------------------------------------------------------------------===//
enum class RelationType : uint8_t {
	INVALID_RELATION,
	TABLE_RELATION,
	PROJECTION_RELATION,
	FILTER_RELATION,
	EXPLAIN_RELATION,
	CROSS_PRODUCT_RELATION,
	JOIN_RELATION,
	AGGREGATE_RELATION,
	SET_OPERATION_RELATION,
	DISTINCT_RELATION,
	LIMIT_RELATION,
	ORDER_RELATION,
	CREATE_VIEW_RELATION,
	CREATE_TABLE_RELATION,
	INSERT_RELATION,
	VALUE_LIST_RELATION,
	DELETE_RELATION,
	UPDATE_RELATION,
	WRITE_CSV_RELATION,
	READ_CSV_RELATION,
	SUBQUERY_RELATION,
	TABLE_FUNCTION_RELATION,
	VIEW_RELATION
};

string RelationTypeToString(RelationType type);

} // namespace graindb
