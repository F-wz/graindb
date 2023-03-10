//===----------------------------------------------------------------------===//
//                         GrainDB
//
// graindb/catalog/catalog_entry/prepared_statement_catalog_entry.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "graindb/catalog/catalog_entry.hpp"
#include "graindb/main/prepared_statement_data.hpp"

namespace graindb {

//! A view catalog entry
class PreparedStatementCatalogEntry : public CatalogEntry {
public:
	PreparedStatementCatalogEntry(string name, unique_ptr<PreparedStatementData> prepared_data)
	    : CatalogEntry(CatalogType::PREPARED_STATEMENT, nullptr, name), prepared(move(prepared_data)) {
	}

	unique_ptr<PreparedStatementData> prepared;
};
} // namespace graindb
