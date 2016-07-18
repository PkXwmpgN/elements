package com.yegorov.alexey.elements.liquid;

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
import com.yegorov.alexey.elements.utils.OffsetInterpreter;
import com.yegorov.alexey.elements.utils.SensorInterpreter;
import com.yegorov.alexey.preferences.PreferenceImagePicker;

public class LiquidWallpaperService extends ElementsWallpaperService
{
    private static final String TAG = "LiquidService";

    @Override
    public Engine onCreateEngine()
    {
        return new LiquidEngine();
    }

    class LiquidEngine extends ElementsEngine
    {
        private static final String DEFAULT_ASSET = "textures/background.png";

        private SensorInterpreter sensorInterpreter = new SensorInterpreter();
        private OffsetInterpreter offsetInterpreter = new OffsetInterpreter();

        private Liquid liquid;

        @Override
        protected void onOffsets(float x, float y, float stepX, float stepY)
        {
            offsetInterpreter.offset(x, y);
        }

        @Override
        protected void onTouch(MotionEvent event)
        {
            if(liquid != null)
                liquid.touch(event.getX(), event.getY(), event.getAction());
        }

        @Override
        protected void onSensor(SensorEvent event)
        {
            if(liquid == null)
                return;

            if(event.sensor.getType() == Sensor.TYPE_ACCELEROMETER)
            {
                float[] delta = offsetInterpreter.accelerometer(getApplicationContext());
                if(delta != null)
                {
                    liquid.acceleration(delta[0], delta[1], 0.0f);
                }
                else
                {
                    delta = sensorInterpreter.accelerometer(getApplicationContext(), event);
                    if(delta != null)
                    {
                        liquid.acceleration(delta[0], delta[1], 0.0f);
                    }
                }
            }
        }

        @Override
        protected void registerSensors(SensorManager sensors, SensorEventListener listener)
        {
            sensors.registerListener(listener, sensors.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),
                    SensorManager.SENSOR_DELAY_FASTEST);

            offsetInterpreter.reset();
            sensorInterpreter.reset();
        }

        @Override
        protected Renderer createRenderer()
        {
            if(liquid != null)
                liquid.close();
            liquid = new Liquid(isPreview());
            return new LiquidRenderer(liquid);
        }

        @Override
        public void onDestroy()
        {
            if(liquid != null)
                liquid.close();
            super.onDestroy();
        }

        class LiquidRenderer implements GLSurfaceView.Renderer
        {
            private Liquid liquid;

            LiquidRenderer(Liquid liquid)
            {
                this.liquid = liquid;
            }

            @Override
            public void onSurfaceCreated(GL10 gl, EGLConfig config)
            {
            }

            @Override
            public void onSurfaceChanged(GL10 gl, int w, int h)
            {
                liquid.startup(w, h, quality());

                loadBackground();
                loadColor();
            }

            @Override
            public void onDrawFrame(GL10 gl)
            {
                liquid.render();
            }

            private void loadBackground()
            {
                final String texture = backgroundTexture();
                if(texture.isEmpty() || !liquid.background(texture))
                {
                    PreferenceImagePicker.prepareFromAsset(getApplicationContext(),
                            getString(R.string.prefs_background_key), DEFAULT_ASSET);
                    liquid.background(backgroundTexture());
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

            private void loadColor()
            {
                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(
                        getApplicationContext());
                final int color = prefs.getInt(getString(R.string.prefs_color_key), 0x55196271);

                liquid.color(Color.red(color) / 255.0f,
                        Color.green(color) / 255.0f,
                        Color.blue(color) / 255.0f,
                        Color.alpha(color) / 255.0f);
            }

            private int quality()
            {
                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(
                        getApplicationContext());
                final String value = prefs.getString(getString(R.string.prefs_quality_key), "1");

                try
                {
                    return Integer.decode(value);
                }
                catch(NumberFormatException ex)
                {
                    Log.e(TAG, "[quality] can't load quality value");
                }
                return 1;
            }
        }
    }
}
