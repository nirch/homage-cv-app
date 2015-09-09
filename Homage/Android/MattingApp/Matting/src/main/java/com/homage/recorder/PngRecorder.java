package com.homage.recorder;



public class PngRecorder {

	public static native int start( String dir );

    public static native int record( int width, int height, byte[] data );


	public static native int  stop();



	static {
        System.loadLibrary("Matting");
    }
}
