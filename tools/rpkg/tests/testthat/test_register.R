library("testthat")
library("DBI")


test_that("graindb_register() works", {
	con <- dbConnect(graindb::graindb())
	
	# most basic case
	graindb::graindb_register(con, "my_df1", iris)
	res <- dbReadTable(con, "my_df1")
	res$Species <- as.factor(res$Species)
	expect_true(identical(res, iris))
	graindb::graindb_unregister(con, "my_df1")

	graindb::graindb_register(con, "my_df2", mtcars)
	res <- dbReadTable(con, "my_df2")
	row.names(res) <- row.names(mtcars)
	expect_true(identical(res, mtcars))
	graindb::graindb_unregister(con, "my_df2")

	graindb::graindb_register(con, "my_df1", mtcars)
	res <- dbReadTable(con, "my_df1")
	row.names(res) <- row.names(mtcars)
	expect_true(identical(res, mtcars))

 	# do not need unregister, can simply overwrite
	graindb::graindb_register(con, "my_df1", iris)
	res <- dbReadTable(con, "my_df1")
	res$Species <- as.factor(res$Species)
	expect_true(identical(res, iris))

	graindb::graindb_unregister(con, "my_df1")
	graindb::graindb_unregister(con, "my_df2")
	graindb::graindb_unregister(con, "xxx")

	# this needs to be empty now
	expect_true(length(attributes(con@conn_ref)) == 0)

	dbDisconnect(con, shutdown=T)
})


test_that("various error cases for graindb_register()", {
	con <- dbConnect(graindb::graindb())

	graindb::graindb_register(con, "my_df1", iris)
	graindb::graindb_unregister(con, "my_df1")
	expect_error(dbReadTable(con, "my_df1"))

	expect_error(graindb::graindb_register(1, "my_df1", iris))
	expect_error(graindb::graindb_register(con, "", iris))
	expect_error(graindb::graindb_unregister(1, "my_df1"))
	expect_error(graindb::graindb_unregister(con, ""))
	dbDisconnect(con, shutdown=T)

	expect_error(graindb::graindb_unregister(con, "my_df1"))

})
