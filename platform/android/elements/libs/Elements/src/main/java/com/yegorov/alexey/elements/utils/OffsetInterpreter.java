package com.yegorov.alexey.elements.utils;

import android.content.Context;
import android.hardware.SensorManager;

import java.util.concurrent.atomic.AtomicBoolean;

public class OffsetInterpreter
{
    private static final int   OFFSETS_DELAY = 500;
    private static final float OFFSETS_INTENSITY = 2.0f;

    private AtomicBoolean activated = new AtomicBoolean(false);
    private java.util.Timer offsetTimer = new java.util.Timer();

    private float offsetValue = 0.0f;
    private float offsetStart = 0.0f;

    private float[] accelerationVector = new float[3];

    public void offset(float x, float y)
    {
        offsetValue = x - offsetStart < 0 ? -OFFSETS_INTENSITY * SensorManager.STANDARD_GRAVITY
                                          :  OFFSETS_INTENSITY * SensorManager.STANDARD_GRAVITY;
        offsetStart = x;

        activated.set(true);

        offsetTimer.cancel();
        offsetTimer = new java.util.Timer();
        offsetTimer.schedule(new java.util.TimerTask()
        {
            @Override
            public void run()
            {
                activated.set(false);
            }
        }, OFFSETS_DELAY);

    }

    public float[] accelerometer(Context context)
    {
        if(activated.get())
        {
            accelerationVector[0] = offsetValue;
            accelerationVector[1] = 0.0f;
            return accelerationVector;
        }
        return null;
    }

    public void reset()
    {
        offsetTimer.cancel();
        activated.set(false);
    }
}
