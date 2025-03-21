#include "catch.hpp"
#include "graindb/common/file_system.hpp"
#include "graindb.hpp"
#include "graindb/main/appender.hpp"
#include "test_helpers.hpp"

#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>

using namespace graindb;
using namespace std;

TEST_CASE("Test transactional integrity when facing process aborts", "[persistence][.]") {
	FileSystem fs;

	// shared memory to keep track of insertions
	size_t *count = (size_t *)mmap(NULL, sizeof(size_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);

	string db_folder_parent = TestCreatePath("llstorage");
	if (fs.DirectoryExists(db_folder_parent)) {
		fs.RemoveDirectory(db_folder_parent);
	}
	fs.CreateDirectory(db_folder_parent);

	string db_folder = fs.JoinPath(db_folder_parent, "dbfolder");
	{
		GrainDB db(db_folder);
		Connection con(db);
		con.Query("CREATE TABLE a (i INTEGER)");
	}

	// fork away a child to be mercilessy shot in a bit
	pid_t pid = fork();

	if (pid == 0) { // child process
		GrainDB db(db_folder);
		Connection con(db);
		while (true) {
			con.Query("INSERT INTO a VALUES(42)");
			(*count)++;
		}
	} else if (pid > 0) { // parent process
		// wait until child has inserted at least 1000 rows
		while (*count < 1000) {
			usleep(100);
		}
		if (kill(pid, SIGKILL) != 0) {
			FAIL();
		}
		unique_ptr<GrainDB> db;
		// it may take some time for the OS to reclaim the lock
		// loop and wait until the database is successfully started again
		for (size_t i = 0; i < 1000; i++) {
			usleep(10000);
			try {
				db = make_unique<GrainDB>(db_folder);
			} catch (...) {
			}
			if (db) {
				break;
			}
		}
		Connection con(*db);
		auto res = con.Query("SELECT COUNT(*) FROM a");
		// there may be an off-by-one if we kill exactly between query and count increment
		REQUIRE(abs((int64_t)(res->GetValue(0, 0).GetValue<int64_t>() - *count)) < 2);
	} else {
		FAIL();
	}
}
