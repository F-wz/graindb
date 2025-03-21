#include "graindb/transaction/transaction_context.hpp"

#include "graindb/common/exception.hpp"
#include "graindb/transaction/transaction.hpp"
#include "graindb/transaction/transaction_manager.hpp"

using namespace graindb;
using namespace std;

TransactionContext::~TransactionContext() {
	if (is_invalidated) {
		return;
	}
	if (current_transaction) {
		try {
			Rollback();
		} catch (...) {
		}
	}
}

void TransactionContext::BeginTransaction() {
	assert(!current_transaction); // cannot start a transaction within a transaction
	current_transaction = transaction_manager.StartTransaction();
}

void TransactionContext::Commit() {
	assert(current_transaction); // cannot commit if there is no active transaction
	auto transaction = current_transaction;
	SetAutoCommit(true);
	current_transaction = nullptr;
	string error = transaction_manager.CommitTransaction(transaction);
	if (!error.empty()) {
		throw TransactionException("Failed to commit: %s", error.c_str());
	}
}

void TransactionContext::Rollback() {
	assert(current_transaction); // cannot rollback if there is no active transaction
	auto transaction = current_transaction;
	SetAutoCommit(true);
	current_transaction = nullptr;
	transaction_manager.RollbackTransaction(transaction);
}
