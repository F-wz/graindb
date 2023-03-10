//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/parser/statement/relation_statement.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/parser/sql_statement.hpp"
#include "graindb/main/relation.hpp"

namespace graindb {

class RelationStatement : public SQLStatement {
public:
	RelationStatement(shared_ptr<Relation> relation) : SQLStatement(StatementType::RELATION_STATEMENT), relation(move(relation)) {
	}

	shared_ptr<Relation> relation;
};

} // namespace graindb
