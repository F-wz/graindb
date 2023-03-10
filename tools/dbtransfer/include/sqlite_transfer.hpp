#include "graindb.hpp"
#include "sqlite3.h"

namespace sqlite {

//! Transfer all data inside the GrainDB connection to the given sqlite database
bool TransferDatabase(graindb::Connection &con, sqlite3 *sqlite);

//! Fires a query to a SQLite database, returning a QueryResult object. Interrupt should be initially set to 0. If
//! interrupt becomes 1 at any point query execution is cancelled.
graindb::unique_ptr<graindb::QueryResult> QueryDatabase(graindb::vector<graindb::SQLType> result_types, sqlite3 *sqlite,
                                                      std::string query, volatile int &interrupt);

}; // namespace sqlite
