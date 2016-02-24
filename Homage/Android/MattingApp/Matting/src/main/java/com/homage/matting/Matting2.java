package com.homage.matting;



public class Matting2 {

	public static native int init( String prmFile, String ctrFile, int width, int height, int threadsCount );
    public static native int release( );

    public static native int getFormattedImage( int threadIdx, byte[] buffer, int width, int height, int orientation, byte[] resRGBAImage);
    public static native int process( int threadIdx, byte[] buffer, int width, int height, int orientation, byte[] resRGBAImage, byte[] resMaskChannel);

    static {
        System.loadLibrary("Matting");
    }
}
