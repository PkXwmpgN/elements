package com.yegorov.alexey.elements.service;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.preference.PreferenceManager;
import android.service.wallpaper.WallpaperService;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.WindowManager;
import com.yegorov.alexey.elements.api.Elements;

public abstract class ElementsWallpaperService extends WallpaperService
{
    @Override
    public void onCreate()
    {
        super.onCreate();

        Elements.initializeAssets(getAssets());
        Elements.initializePreferences(PreferenceManager.getDefaultSharedPreferences(getApplicationContext()));

        WindowManager wm = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
        DisplayMetrics metrics = new DisplayMetrics();
        wm.getDefaultDisplay().getMetrics(metrics);

        Elements.initializeMetrics(metrics);
    }

    protected abstract class ElementsEngine extends Engine
            implements SensorEventListener
    {
        private ElementSV elementSurface;
        private SensorManager sensors;

        private class ElementSV extends GLSurfaceView
        {
            ElementSV(Context context)
            {
                super(context);
            }

            @Override
            public SurfaceHolder getHolder()
            {
                return getSurfaceHolder();
            }

            public void onDestroy()
            {
                onDetachedFromWindow();
            }
        }

        @Override
        public void onCreate(SurfaceHolder surfaceHolder)
        {
            super.onCreate(surfaceHolder);

            sensors = (SensorManager)getSystemService(Context.SENSOR_SERVICE);

            setTouchEventsEnabled(true);
            setOffsetNotificationsEnabled(true);

            elementSurface = new ElementSV(ElementsWallpaperService.this);
            elementSurface.setEGLContextClientVersion(2);
            elementSurface.setEGLConfigChooser(8, 8, 8, 8, getDepth(), 0);
            elementSurface.setPreserveEGLContextOnPause(true);
            elementSurface.setRenderer(createRenderer());
        }

        @Override
        public void onTouchEvent(MotionEvent event)
        {
            onTouch(event);
        }

        @Override
        public void onOffsetsChanged(float xOffset, float yOffset,
                                     float xOffsetStep, float yOffsetStep,
                                     int xPixelOffset, int yPixelOffset)
        {
            if(!isPreview())
                onOffsets(xOffset, yOffset, xOffsetStep, yOffsetStep);
        }

        @Override
        public void onVisibilityChanged(boolean visible)
        {
            super.onVisibilityChanged(visible);
            if(visible)
            {
                elementSurface.onResume();
                registerSensors(sensors, this);
            }
            else
            {
                elementSurface.onPause();
                sensors.unregisterListener(this);
            }
        }

        @Override
        public void onSensorChanged(SensorEvent event)
        {
            onSensor(event);
        }

        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy)
        {

        }

        @Override
        public void onDestroy()
        {
            super.onDestroy();
            elementSurface.onDestroy();
            sensors.unregisterListener(this);
        }


        protected int getDepth()
        {
            return 0;
        }

        abstract protected void onOffsets(float x, float y, float stepX, float stepY);
        abstract protected void onTouch(MotionEvent event);
        abstract protected void onSensor(SensorEvent event);

        abstract protected void registerSensors(SensorManager sensors, SensorEventListener listener);
        abstract protected Renderer createRenderer();
    }
}



