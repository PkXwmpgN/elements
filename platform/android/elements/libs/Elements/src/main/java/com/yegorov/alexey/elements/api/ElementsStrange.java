package com.yegorov.alexey.elements.api;

public class ElementsStrange
{
    static
    {
        System.loadLibrary("elements");
    }

    public static native int create(boolean preview);
    public static native boolean startup(int id, int width, int height, int textureSize);
    public static native boolean destroy(int id);
    public static native boolean background(int id, float r, float g, float b);
    public static native boolean gradient(int id, float r1, float g1, float b1, float r2, float g2, float b2);
    public static native boolean render(int id);
}
