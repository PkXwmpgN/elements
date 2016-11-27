package com.yegorov.alexey.elements.strange;

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

public class StrangeWallpaperService extends ElementsWallpaperService
{
    private static final String TAG = "StrangeService";

    @Override
    public Engine onCreateEngine()
    {
        return new StrangeEngine();
    }

    class StrangeEngine extends ElementsEngine
    {
        private Strange strange;

        @Override
        protected void onOffsets(float x, float y, float stepX, float stepY)
        {
        }

        @Override
        protected void onTouch(MotionEvent event)
        {
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
            if(strange != null)
                strange.close();
            strange = new Strange(isPreview());
            return new StrangeRenderer(strange);
        }

        @Override
        public void onDestroy()
        {
            if(strange != null)
                strange.close();
            super.onDestroy();
        }

        class StrangeRenderer extends ElementsRenderer
        {
            private Strange strange;

            StrangeRenderer(Strange strange)
            {
                this.strange = strange;
            }

            @Override
            public void onSurfaceChanged(GL10 gl, int w, int h)
            {
                super.onSurfaceChanged(gl, w, h);
                strange.startup(w, h, quantity());
                loadColors();
            }

            @Override
            public void onDrawFrame(GL10 gl)
            {
                strange.render();
            }


            private void loadColors()
            {
                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(
                        getApplicationContext());

                final int colorBackground = prefs.getInt(getString(R.string.prefs_color_background_key), 0xff202020);
                strange.colorBackground(Color.red(colorBackground) / 255.0f,
                        Color.green(colorBackground) / 255.0f,
                        Color.blue(colorBackground) / 255.0f);

                final int gradient1 = prefs.getInt(getString(R.string.prefs_color_gradient_1_key), 0xff747474);
                final int gradient2 = prefs.getInt(getString(R.string.prefs_color_gradient_2_key), 0xfff9f517);

                strange.colorGradient(Color.red(gradient1) / 255.0f,
                        Color.green(gradient1) / 255.0f,
                        Color.blue(gradient1) / 255.0f,
                        Color.red(gradient2) / 255.0f,
                        Color.green(gradient2) / 255.0f,
                        Color.blue(gradient2) / 255.0f);
            }


            private int quantity()
            {
                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(
                        getApplicationContext());
                final String value = prefs.getString(getString(R.string.prefs_quantity_key), "500000");

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
