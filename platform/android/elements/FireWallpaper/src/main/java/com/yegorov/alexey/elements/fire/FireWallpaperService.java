package com.yegorov.alexey.elements.fire;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.SharedPreferences;
import android.content.Context;

import android.graphics.Color;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;

import android.hardware.SensorEventListener;
import android.hardware.SensorEvent;
import android.hardware.SensorManager;

import android.preference.PreferenceManager;

import android.util.DisplayMetrics;

import android.util.Log;
import android.view.MotionEvent;
import android.view.WindowManager;


import com.yegorov.alexey.elements.service.ElementsWallpaperService;
import com.yegorov.alexey.preferences.PreferenceImagePicker;

public class FireWallpaperService extends ElementsWallpaperService
{
    private static final String TAG = "FireService";

    @Override
    public Engine onCreateEngine()
    {
        return new FireEngine();
    }

    class FireEngine extends ElementsEngine
    {
        private static final String DEFAULT_ASSET = "textures/background.png";

        private Fire fire;

        @Override
        protected void onOffsets(float x, float y, float stepX, float stepY)
        {
        }

        @Override
        protected void onTouch(MotionEvent event)
        {
            if(fire != null)
                fire.touch(event.getX(), event.getY(), event.getAction());
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
            if(fire != null)
                fire.close();
            fire = new Fire(isPreview());
            return new FireRenderer(fire);
        }

        @Override
        public void onDestroy()
        {
            fire.close();
            super.onDestroy();
        }

        class FireRenderer extends ElementsRenderer
        {
            private Fire fire;

            FireRenderer(Fire fire)
            {
                this.fire = fire;
            }

            @Override
            public void onSurfaceChanged(GL10 gl, int w, int h)
            {
                super.onSurfaceChanged(gl, w, h);
                fire.startup(w, h, quality());

                loadBackground();
                loadColors();
            }

            @Override
            public void onDrawFrame(GL10 gl)
            {
                fire.render();
            }

            private void loadBackground()
            {
                final String texture = backgroundTexture();
                if(texture.isEmpty() || !fire.background(texture))
                {
                    PreferenceImagePicker.prepareFromAsset(getApplicationContext(),
                            getString(R.string.prefs_background_key), DEFAULT_ASSET);
                    fire.background(backgroundTexture());
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

            private void loadColors()
            {
                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(
                        getApplicationContext());
                final int colorHot = prefs.getInt(getString(R.string.prefs_color_hot_key), 0xff1980ff);
                fire.colorHot(Color.red(colorHot) / 255.0f,
                        Color.green(colorHot) / 255.0f,
                        Color.blue(colorHot) / 255.0f);

                final int colorCold = prefs.getInt(getString(R.string.prefs_color_cold_key), 0xffff8019);
                fire.colorCold(Color.red(colorCold) / 255.0f,
                        Color.green(colorCold) / 255.0f,
                        Color.blue(colorCold) / 255.0f);
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
