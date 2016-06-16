package com.yegorov.alexey.elements.api;

import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.util.DisplayMetrics;

public class Elements
{
    static
    {
        System.loadLibrary("elements");
    }

    public static native void initializeAssets(AssetManager manager);
    public static native void initializePreferences(SharedPreferences preferences);
    public static native void initializeMetrics(DisplayMetrics metrics);
}
