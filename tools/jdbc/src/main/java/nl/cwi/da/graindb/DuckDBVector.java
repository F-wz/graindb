package nl.cwi.da.graindb;

import java.nio.ByteBuffer;

public class GrainDBVector {
	
	public GrainDBVector(String graindb_type, int length,  boolean[] nullmask) {
		super();
		this.graindb_type = graindb_type;
		this.length = length;
		this.nullmask = nullmask;
	}
	protected String graindb_type;
	protected int length;
	protected boolean[] nullmask;
	protected ByteBuffer constlen_data = null;
	protected Object[] varlen_data = null;

}
