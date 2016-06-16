package com.yegorov.alexey.elements.api;

public class ElementsAir
{
    static
    {
        System.loadLibrary("elements");
    }

    public static native int create(boolean preview);
    public static native boolean destroy(int id);
    public static native boolean startup(int id, int width, int height, int textureSize);
    public static native boolean colors(int id, float r0, float g0, float b0, float r1, float g1, float b1);
    public static native boolean touch(int id, float x, float y, int action);
    public static native boolean render(int id);
}
