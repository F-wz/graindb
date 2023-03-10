package nl.cwi.da.graindb;

import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.sql.SQLException;

public class GrainDBDatabase {
	
	protected String url;

	public GrainDBDatabase(String url) throws SQLException {
		if (!url.startsWith("jdbc:graindb")) {
			throw new SQLException("GrainDB JDBC URL needs to start with 'jdbc:graindb:'");
		}
		this.url = url;
		String db_dir = url.replaceFirst("^jdbc:graindb:", "").trim();
		if (db_dir.length() == 0) {
			db_dir = ":memory:";
		}
		db_ref = GrainDBNative.graindb_jdbc_startup(db_dir.getBytes(StandardCharsets.UTF_8), false);
	}
	
	protected synchronized void finalize() throws Throwable {
		if (db_ref != null) {
			GrainDBNative.graindb_jdbc_shutdown(db_ref);
			db_ref = null;
		}
	}
	
	protected ByteBuffer db_ref;

}
