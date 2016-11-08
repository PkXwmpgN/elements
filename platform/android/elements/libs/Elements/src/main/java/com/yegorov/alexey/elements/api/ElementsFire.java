package com.yegorov.alexey.elements.api;

public class ElementsFire
{
    static
    {
        System.loadLibrary("experiments");
    }

    public static native int  create(boolean preview);
    public static native boolean startup(int id, int width, int height, int textureSize);
    public static native boolean destroy(int id);
    public static native boolean background(int id, String asset);
    public static native boolean colorHot(int id, float r, float g, float b);
    public static native boolean colorCold(int id, float r, float g, float b);
    public static native boolean touch(int id, float x, float y, int action);
    public static native boolean render(int id);
}
