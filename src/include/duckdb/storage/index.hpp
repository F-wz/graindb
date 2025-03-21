//===----------------------------------------------------------------------===//
//                         GrainDB
//
// duckdb/storage/index.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/unordered_set.hpp"
#include "duckdb/common/enums/index_type.hpp"
#include "duckdb/common/types/data_chunk.hpp"
#include "duckdb/parser/parsed_expression.hpp"
#include "duckdb/planner/expression.hpp"
#include "duckdb/storage/table/scan_state.hpp"
#include "duckdb/execution/expression_executor.hpp"

namespace duckdb {

class ClientContext;
class Transaction;

struct IndexLock;

//! The index is an abstract base class that serves as the basis for indexes
class Index {
public:
	Index(IndexType type, vector<column_t> column_ids,
	      vector<unique_ptr<Expression>> unbound_expressions);
	virtual ~Index() = default;

	//! Lock used for updating the index
	std::mutex lock;
	//! The type of the index
	IndexType type;
	//! Column identifiers to extract from the base table
	vector<column_t> column_ids;
	//! unordered_set of column_ids used by the index
	unordered_set<column_t> column_id_set;
	//! Unbound expressions used by the index
	vector<unique_ptr<Expression>> unbound_expressions;
	//! The types of the expressions
	vector<TypeId> types;

public:
	//! Initialize a scan on the index with the given expression and column ids
	//! to fetch from the base table when we only have one query predicate
	virtual unique_ptr<IndexScanState> InitializeScanSinglePredicate(Transaction &transaction,
	                                                                 vector<column_t> column_ids, Value value,
	                                                                 ExpressionType expressionType) = 0;
	//! Initialize a scan on the index with the given expression and column ids
	//! to fetch from the base table for two query predicates
	virtual unique_ptr<IndexScanState> InitializeScanTwoPredicates(Transaction &transaction,
	                                                               vector<column_t> column_ids, Value low_value,
	                                                               ExpressionType low_expression_type, Value high_value,
	                                                               ExpressionType high_expression_type) = 0;
	//! Perform a lookup on the index
	virtual void Scan(Transaction &transaction, DataTable &table, TableIndexScanState &state, DataChunk &result) = 0;

	//! Obtain a lock on the index
	virtual void InitializeLock(IndexLock &state);
	//! Called when data is appended to the index. The lock obtained from InitializeAppend must be held
	virtual bool Append(IndexLock &state, DataChunk &entries, Vector &row_identifiers) = 0;
	bool Append(DataChunk &entries, Vector &row_identifiers);
	//! Verify that data can be appended to the index
	virtual void VerifyAppend(DataChunk &chunk) {
	}

	//! Called when data inside the index is Deleted
	virtual void Delete(IndexLock &state, DataChunk &entries, Vector &row_identifiers) = 0;
	void Delete(DataChunk &entries, Vector &row_identifiers);

	//! Insert data into the index. Does not lock the index.
	virtual bool Insert(IndexLock &lock, DataChunk &input, Vector &row_identifiers) = 0;

	//! Returns true if the index is affected by updates on the specified column ids, and false otherwise
	bool IndexIsUpdated(vector<column_t> &column_ids);

protected:
	void ExecuteExpressions(DataChunk &input, DataChunk &result);

private:
	//! Bound expressions used by the index
	vector<unique_ptr<Expression>> bound_expressions;
	//! Expression executor for the index expressions
	ExpressionExecutor executor;

	unique_ptr<Expression> BindExpression(unique_ptr<Expression> expr);
};

} // namespace duckdb
