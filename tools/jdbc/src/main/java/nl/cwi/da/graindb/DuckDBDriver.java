package nl.cwi.da.graindb;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.DriverPropertyInfo;
import java.sql.SQLException;
import java.sql.SQLFeatureNotSupportedException;
import java.util.Properties;
import java.util.logging.Logger;

public class GrainDBDriver implements java.sql.Driver {

	static {
		try {
			DriverManager.registerDriver(new GrainDBDriver());
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

	public Connection connect(String url, Properties info) throws SQLException {
		GrainDBDatabase db = new GrainDBDatabase(url);
		return new GrainDBConnection(db);
	}

	public boolean acceptsURL(String url) throws SQLException {
		return url.startsWith("jdbc:graindb:");
	}

	public DriverPropertyInfo[] getPropertyInfo(String url, Properties info) throws SQLException {
		DriverPropertyInfo[] ret = {};
		return ret; // no properties
	}

	public int getMajorVersion() {
		return 1;
	}

	public int getMinorVersion() {
		return 0;
	}

	public boolean jdbcCompliant() {
		return true; // of course!
	}

	public Logger getParentLogger() throws SQLFeatureNotSupportedException {
		throw new SQLFeatureNotSupportedException("no logger");
	}

}
