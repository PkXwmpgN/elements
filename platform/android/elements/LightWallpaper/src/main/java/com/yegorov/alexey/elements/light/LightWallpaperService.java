package com.yegorov.alexey.elements.light;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.SharedPreferences;
import android.content.Context;

import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;

import android.hardware.SensorEventListener;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorManager;

import android.graphics.Color;
import android.preference.PreferenceManager;

import android.util.DisplayMetrics;

import android.util.Log;
import android.view.MotionEvent;
import android.view.WindowManager;

import com.yegorov.alexey.elements.service.ElementsWallpaperService;
import com.yegorov.alexey.preferences.PreferenceImagePicker;

public class LightWallpaperService extends ElementsWallpaperService
{
    private static final String TAG = "LightService";

    @Override
    public Engine onCreateEngine()
    {
        return new LightEngine();
    }

    class LightEngine extends ElementsEngine
    {
        private static final String DEFAULT_ASSET = "textures/background.png";

        private Light light;

        @Override
        protected void onOffsets(float x, float y, float stepX, float stepY)
        {
        }

        @Override
        protected void onTouch(MotionEvent event)
        {
            if(light != null)
                light.surfaceTouch(event.getX(), event.getY(), event.getAction());
        }

        @Override
        protected void onSensor(SensorEvent event)
        {
        }

        @Override
        protected void registerSensors(SensorManager sensors, SensorEventListener listener)
        {
            sensors.registerListener(listener, sensors.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),
                    SensorManager.SENSOR_DELAY_FASTEST);

        }

        @Override
        protected Renderer createRenderer()
        {
            if(light != null)
                light.close();
            light = new Light(isPreview());
            return new LightRenderer(light);
        }

        @Override
        public void onDestroy()
        {
            if(light != null)
                light.close();
            super.onDestroy();
        }

        class LightRenderer implements GLSurfaceView.Renderer
        {
            private Light light;

            LightRenderer(Light light)
            {
                this.light = light;
            }

            @Override
            public void onSurfaceCreated(GL10 gl, EGLConfig config)
            {
            }

            @Override
            public void onSurfaceChanged(GL10 gl, int w, int h)
            {
                light.startup(w, h, quantity());

                loadSurfaceBackground();
                loadSurfaceColor();
            }

            @Override
            public void onDrawFrame(GL10 gl)
            {
                light.render();
            }

            private void loadSurfaceBackground()
            {
                final String texture = backgroundTexture();
                if(texture.isEmpty() || !light.background(texture))
                {
                    PreferenceImagePicker.prepareFromAsset(getApplicationContext(),
                            getString(R.string.prefs_background_key), DEFAULT_ASSET);
                    light.background(backgroundTexture());
                }
            }

            private String backgroundTexture()
            {
                WindowManager wm = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
                DisplayMetrics metrics = new DisplayMetrics();
                wm.getDefaultDisplay().getMetrics(metrics);

                final String key = metrics.heightPixels < metrics.widthPixels ?
                                   getString(R.string.prefs_background_key) + PreferenceImagePicker.LANDSCAPE_SUFFIX :
                                   getString(R.string.prefs_background_key) + PreferenceImagePicker.PORTRAIT_SUFFIX;

                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
                return prefs.getString(key, "");
            }

            private void loadSurfaceColor()
            {
                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(
                        getApplicationContext());
                final int color = prefs.getInt(getString(R.string.prefs_color_key), 0xFFFFFFFF);

                light.color(Color.red(color) / 255.0f,
                        Color.green(color) / 255.0f,
                        Color.blue(color) / 255.0f);
            }

            private int quantity()
            {
                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(
                        getApplicationContext());
                final String value = prefs.getString(getString(R.string.prefs_quantity_key), "1000");

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
