package com.homage.matting;



public class Matting2 {

    public static synchronized int createSafe(String prmFile, String ctrFile, int width, int height) {
        int id = create(prmFile, ctrFile, width, height);
        return id;
    }

    public static native int create(String prmFile, String ctrFile, int width, int height);
    public static native int delete(int id);

    public static native int yuv2rgb(int id, byte[] buffer, int width, int height, int orientation, boolean crop, byte[] resRGBImage);
    public static native int inspectRGB(int id, byte[] buffer, int width, int height);
    public static native int inspectYUV(int id, byte[] buffer, int width, int height, int orientation, byte[]resRGBImage);
    public static native int removeBackgroundRGB(int id, byte[] buffer, int width, int height, byte[] resMaskChannel);
    public static native int removeBackgroundYUV(int id, byte[] buffer, int width, int height, int orientation, byte[]resRGBImage, byte[] resMaskChannel);

    static {
        System.loadLibrary("Matting");
    }
}
