package com.yegorov.alexey.elements.utils;

import android.content.Context;
import android.hardware.SensorEvent;
import android.hardware.SensorManager;
import android.view.Display;
import android.view.Surface;
import android.view.WindowManager;

import static android.hardware.SensorManager.AXIS_MINUS_X;
import static android.hardware.SensorManager.AXIS_MINUS_Y;
import static android.hardware.SensorManager.AXIS_X;
import static android.hardware.SensorManager.AXIS_Y;

public class SensorInterpreter
{
    private float[] tiltVector = new float[3];
    private float[] accelerationVector = new float[3];

    private float[] rotationMatrixTarget = null;
    private float[] rotationMatrix = new float[16];
    private float[] rotationMatrixOriented = new float[16];

    private float[] valuesTruncated;

    private Context context;
    private int rotation = Surface.ROTATION_0;

    public SensorInterpreter(Context context)
    {
        this.context = context;
    }

    public float[] rotation(SensorEvent event)
    {
        if(event == null)
            return null;

        float[] values = getSensorEventValues(event);

        if(rotationMatrixTarget == null)
        {
            rotationMatrixTarget = new float[16];
            SensorManager.getRotationMatrixFromVector(rotationMatrixTarget, values);
            return null;
        }

        SensorManager.getRotationMatrixFromVector(rotationMatrix, values);
        if(rotation == Surface.ROTATION_0)
        {
            SensorManager.getAngleChange(tiltVector, rotationMatrix, rotationMatrixTarget);
        }
        else
        {
            switch(rotation)
            {
                case Surface.ROTATION_90:
                    SensorManager.remapCoordinateSystem(rotationMatrix, AXIS_Y, AXIS_MINUS_X, rotationMatrixOriented);
                    break;

                case Surface.ROTATION_180:
                    SensorManager.remapCoordinateSystem(rotationMatrix, AXIS_MINUS_X, AXIS_MINUS_Y, rotationMatrixOriented);
                    break;

                case Surface.ROTATION_270:
                    SensorManager.remapCoordinateSystem(rotationMatrix, AXIS_MINUS_Y, AXIS_X, rotationMatrixOriented);
                    break;
            }

            SensorManager.getAngleChange(tiltVector, rotationMatrixOriented, rotationMatrixTarget);
        }

        tiltVector[1] = (float)(Math.PI / 2.0 + tiltVector[1]);
        tiltVector[2] = -tiltVector[2];

        return tiltVector;
    }

    public float[] accelerometer(SensorEvent event)
    {
        if(event == null)
            return null;

        float[] values = getSensorEventValues(event);

        if(rotation == Surface.ROTATION_0)
        {
            accelerationVector[0] = -values[0];
            accelerationVector[1] = -values[1];
        }
        else if(rotation == Surface.ROTATION_90)
        {
            accelerationVector[0] = values[1];
            accelerationVector[1] = -values[0];
        }
        else if(rotation == Surface.ROTATION_180)
        {
            accelerationVector[0] = values[0];
            accelerationVector[1] = values[1];
        }
        else if(rotation == Surface.ROTATION_270)
        {
            accelerationVector[0] = -values[1];
            accelerationVector[1] = values[0];
        }

        return accelerationVector;
    }

    float[] getSensorEventValues(SensorEvent event)
    {
        if(event.values.length > 4)
        {
            if (valuesTruncated == null)
                valuesTruncated = new float[4];
            System.arraycopy(event.values, 0, valuesTruncated, 0, 4);
            return valuesTruncated;
        }

        return event.values;
    }

    public void reset()
    {
        rotationMatrixTarget = null;
        rotation = ((WindowManager) context
                .getSystemService(Context.WINDOW_SERVICE))
                .getDefaultDisplay().getRotation();
    }
}
