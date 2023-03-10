# cursor description

class TestCursorDescription(object):
    def test_description(self, graindb_cursor):
        description = graindb_cursor.execute("SELECT * FROM integers").description
        assert description == [('i', None, None, None, None, None, None)]
