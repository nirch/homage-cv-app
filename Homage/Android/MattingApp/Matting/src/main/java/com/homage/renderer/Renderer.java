package com.homage.renderer;



public class Renderer {

	public static native int create();


	public static native int addSource( int sourceType, String file );
    public static native int addSourceFrame( int sourceType, String file, int[] af );

    public static native int addEffect( int sourceId, int effectType, String file );

    public static native int addEffect( int sourceId, int effect, byte[] data );

    public static native int addOutput( int outputType, String file, int width, int height, int duration  );

    /*public static native int addJavaOutput( Object videoEncoder, String file, int width, int height, int frameRate  );
    public static native int addJavaOutputA( String className, String file, int width, int height, int frameRate  );*/

    public static native int setOutputPallete( int outputId, byte[] data );


    public static native int process();

	public static native int  getStatus();



	static {
        System.loadLibrary("Matting");
    }
}
