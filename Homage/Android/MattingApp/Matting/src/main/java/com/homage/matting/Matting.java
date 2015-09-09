package com.homage.matting;



public class Matting {

	public static native int openLog( String logFile );
	public static native int closeLog();
	
	
	public static native int init( String prmFile, String ctrFile, int width, int height );

    public static native int setFrameRate( int framerate );

    public static native int setBackground( String file );

    public static native int setDumpDir( String file );

	public static native int initCamera( int orientation );
	
	public static native int cameraCallBack( int width, int height, byte[] data );

	public static native int  getUpdate( byte[] out,  byte[] out1 );
	
	
	public static native int start();
	
	public static native int stop();
	
	public static native int setOperation( int operation );
	
	public static native int  getStatus();


    public static native int startRecord( String dir );
    public static native int stopRecord();

	static {
        System.loadLibrary("Matting");
    }
}
