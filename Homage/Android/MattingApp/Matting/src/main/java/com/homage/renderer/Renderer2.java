package com.homage.renderer;



public class Renderer2 {



    /*public static native int init();
    public static native int calc(Object obj);
    public static native int writeFrame(Object obj);
    public static native int readFrame(Object obj);*/

    public static synchronized int createSafe(float duration, int fps) {
        int id = create(duration, fps);
        return id;
    }

    public static native int create(float duration, int fps);

    public static native int addImageSource( int id, int imageType, String file );

    public static native int addFX(int id, int sourceId, int effectType, String data );

    public static native int addOutput(int id, int outputType, String file, int width, int height, int duration  );

    public static native int process(int id, int width, int height);
    public static native int delete(int id);

	static {
        System.loadLibrary("Matting");
    }
}
