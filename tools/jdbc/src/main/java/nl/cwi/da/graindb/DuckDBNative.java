package nl.cwi.da.graindb;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;

public class GrainDBNative {
	static {
		String libName = "libgraindb_java.so";
		try {
			Path libFile = Files.createTempFile("libgraindb_java", ".so");
			InputStream in = GrainDBNative.class.getResource("/" + libName).openStream();
			Files.copy(in, libFile, StandardCopyOption.REPLACE_EXISTING);
			new File(libFile.toString()).deleteOnExit();
			System.load(libFile.toString());
		} catch (IOException e) {
			e.printStackTrace();
		}

//		System.loadLibrary("graindb_java");
	}
	// We use zero-length ByteBuffer-s as a hacky but cheap way to pass C++ pointers
	// back and forth

	/*
	 * NB: if you change anything below, run `javah` on this class to re-generate
	 * the C header
	 */

	// results db_ref database reference object
	protected static native ByteBuffer graindb_jdbc_startup(byte[] path, boolean read_only);

	protected static native void graindb_jdbc_shutdown(ByteBuffer db_ref);

	// returns conn_ref connection reference object
	protected static native ByteBuffer graindb_jdbc_connect(ByteBuffer db_ref);

	protected static native void graindb_jdbc_set_auto_commit(ByteBuffer conn_ref, boolean auto_commit);

	protected static native boolean graindb_jdbc_get_auto_commit(ByteBuffer conn_ref);

	protected static native void graindb_jdbc_disconnect(ByteBuffer conn_ref);

	// returns stmt_ref result reference object
	protected static native ByteBuffer graindb_jdbc_prepare(ByteBuffer conn_ref, byte[] query);

	protected static native String graindb_jdbc_prepare_type(ByteBuffer stmt_ref);

	protected static native void graindb_jdbc_release(ByteBuffer stmt_ref);

	protected static native GrainDBResultSetMetaData graindb_jdbc_meta(ByteBuffer stmt_ref);

	
	// returns res_ref result reference object
	protected static native ByteBuffer graindb_jdbc_execute(ByteBuffer stmt_ref, Object[] params);


	protected static native void graindb_jdbc_free_result(ByteBuffer res_ref);

	protected static native GrainDBVector[] graindb_jdbc_fetch(ByteBuffer res_ref);
	
	protected static native int graindb_jdbc_fetch_size();
}
