package com.yegorov.alexey.elements.air;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.SharedPreferences;

import android.graphics.Color;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;

import android.hardware.SensorEventListener;
import android.hardware.SensorEvent;
import android.hardware.SensorManager;

import android.preference.PreferenceManager;

import android.util.Log;
import android.view.MotionEvent;

import com.yegorov.alexey.elements.service.ElementsWallpaperService;

public class AirWallpaperService extends ElementsWallpaperService
{
    private static final String TAG = "AirService";

    @Override
    public Engine onCreateEngine()
    {
        return new AirEngine();
    }

    class AirEngine extends ElementsEngine
    {
        private static final String DEFAULT_ASSET = "common/images/background.png";

        private Air air;

        @Override
        protected void onOffsets(float x, float y, float stepX, float stepY)
        {
        }

        @Override
        protected void onTouch(MotionEvent event)
        {
            if(air != null)
                air.touch(event.getX(), event.getY(), event.getAction());
        }

        @Override
        protected void onSensor(SensorEvent event)
        {
        }

        @Override
        protected void registerSensors(SensorManager sensors, SensorEventListener listener)
        {
        }

        @Override
        protected Renderer createRenderer()
        {
            if(air != null)
                air.close();
            air = new Air(isPreview());
            return new AirRenderer(air);
        }

        @Override
        public void onDestroy()
        {
            if(air != null)
                air.close();
            super.onDestroy();
        }

        class AirRenderer implements GLSurfaceView.Renderer
        {
            private Air air;

            AirRenderer(Air air)
            {
                this.air = air;
            }

            @Override
            public void onSurfaceCreated(GL10 gl, EGLConfig config)
            {
            }

            @Override
            public void onSurfaceChanged(GL10 gl, int w, int h)
            {
                air.startup(w, h, quantity());
                loadColors();
            }

            @Override
            public void onDrawFrame(GL10 gl)
            {
                air.render();
            }


            private void loadColors()
            {
                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(
                        getApplicationContext());

                final int colorSpeedDown = prefs.getInt(getString(R.string.prefs_color_speed_down_key), 0xFF0091EA);
                final int colorSpeedUp = prefs.getInt(getString(R.string.prefs_color_speed_up_key), 0xFFFF6F00);
                air.colors(Color.red(colorSpeedDown) / 255.0f,
                        Color.green(colorSpeedDown) / 255.0f,
                        Color.blue(colorSpeedDown) / 255.0f,
                        Color.red(colorSpeedUp) / 255.0f,
                        Color.green(colorSpeedUp) / 255.0f,
                        Color.blue(colorSpeedUp) / 255.0f);

            }

            private int quantity()
            {
                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(
                        getApplicationContext());
                final String value = prefs.getString(getString(R.string.prefs_quantity_key), "1000000");

                try
                {
                    return Integer.decode(value);
                }
                catch(NumberFormatException ex)
                {
                    Log.e(TAG, "[quantity] can't load quantity value");
                }
                return 1000;
            }
        }
    }
    @Override
    public void onCreate()
    {
        super.onCreate();
    }

    @Override
    public void onDestroy()
    {
        super.onDestroy();
    }
}
