#simple DB API testcase

import pandas as pd

class TestSimpleDBAPI(object):
	def test_to_sql(self, graindb_cursor):
		con = graindb_cursor
		df = pd.DataFrame([[None, 10, 1.0], ['nick', None, 1.5], ['juli', 14, None]], columns = ['Name', 'Age', 'Numeric'])
		df.to_sql('ages', con)

		result = graindb_cursor.execute('SELECT count(*), sum("Age"), sum("Numeric") FROM ages').fetchone()
		assert result == (3, 24, 2.5,)
