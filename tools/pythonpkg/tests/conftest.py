import numpy
import os
import pytest
import shutil

import graindb

@pytest.fixture(scope="function")
def graindb_empty_cursor(request):
    test_dbfarm = tmp_path.resolve().as_posix()

    connection = graindb.connect('')
    cursor = connection.cursor()
    return cursor

@pytest.fixture(scope="function")
def graindb_cursor(request):

    connection = graindb.connect('')
    cursor = connection.cursor()
   # cursor.create('integers', {'i': numpy.arange(10)})
    cursor.execute('CREATE TABLE integers (i integer)')
    cursor.execute('INSERT INTO integers VALUES (0),(1),(2),(3),(4),(5),(6),(7),(8),(9),(NULL)')
    cursor.execute('CREATE TABLE timestamps (t timestamp)')
    cursor.execute("INSERT INTO timestamps VALUES ('1992-10-03 18:34:45'), ('2010-01-01 00:00:01'), (NULL)")
    return cursor


@pytest.fixture(scope="function")
def graindb_cursor_autocommit(request, tmp_path):
    test_dbfarm = tmp_path.resolve().as_posix()

    def finalizer():
        graindb.shutdown()
        if tmp_path.is_dir():
            shutil.rmtree(test_dbfarm)

    request.addfinalizer(finalizer)

    connection = graindb.connect(test_dbfarm)
    connection.set_autocommit(True)
    cursor = connection.cursor()
    return (cursor, connection, test_dbfarm)


@pytest.fixture(scope="function")
def initialize_graindb(request, tmp_path):
    test_dbfarm = tmp_path.resolve().as_posix()

    def finalizer():
        graindb.shutdown()
        if tmp_path.is_dir():
            shutil.rmtree(test_dbfarm)

    request.addfinalizer(finalizer)

    graindb.connect(test_dbfarm)
    return test_dbfarm
