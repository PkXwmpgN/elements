package com.yegorov.alexey.elements.api;

public class ElementsCharacter
{
    static
    {
        System.loadLibrary("elements");
    }

    public static native int create(boolean preview);
    public static native boolean startup(int id, int width, int height);
    public static native boolean destroy(int id);
    public static native boolean render(int id);
    public static native boolean rotation(int id, float x, float y, float z);
    public static native boolean model(int id, String model);
}
