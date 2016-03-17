package com.homage.matting;



public class Matting2 {

    public static synchronized int createSafe(String prmFile, String ctrFile, int width, int height) {
        int id = create(prmFile, ctrFile, width, height);
        return id;
    }

    public static native int create(String prmFile, String ctrFile, int width, int height);
    public static native int delete(int id);

    public static native int prepareYUV(int id, byte[] buffer, int width, int height, int orientation, byte[] resRGBImage);
    public static native int prepareRGB(int id, byte[] buffer, int width, int height, byte[] resRGBImage);
    public static native int getBackgroundSimilarity(int id);
    public static native int inspect(int id);

    public static native int process(int id, byte[] resMaskChannel);

    static {
        System.loadLibrary("Matting");
    }
}
