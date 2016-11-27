package com.yegorov.alexey.elements.character;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.SharedPreferences;
import android.hardware.Sensor;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;

import android.hardware.SensorEventListener;
import android.hardware.SensorEvent;
import android.hardware.SensorManager;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.MotionEvent;

import com.yegorov.alexey.elements.service.ElementsWallpaperService;
import com.yegorov.alexey.elements.utils.SensorInterpreter;

public class CharacterWallpaperService extends ElementsWallpaperService
{
    private static final String TAG = "CharacterService";

    @Override
    public Engine onCreateEngine()
    {
        return new CharacterEngine();
    }

    class CharacterEngine extends ElementsEngine
    {
        private Character character;
        private SensorInterpreter sensorInterpreter;

        CharacterEngine()
        {
            sensorInterpreter = new SensorInterpreter(getApplicationContext());
        }
        @Override
        protected int getDepth()
        {
            return 24;
        }

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
            if(character == null)
                return;

            if(event.sensor.getType() == Sensor.TYPE_GAME_ROTATION_VECTOR)
            {
                float [] delta = sensorInterpreter.rotation(event);
                if(delta != null)
                {
                    character.rotation(delta[0], delta[1], delta[2]);
                }
            }
        }

        @Override
        protected void registerSensors(SensorManager sensors, SensorEventListener listener)
        {
            sensors.registerListener(listener, sensors.getDefaultSensor(Sensor.TYPE_GAME_ROTATION_VECTOR),
                    SensorManager.SENSOR_DELAY_FASTEST);

            sensorInterpreter.reset();
        }

        @Override
        protected Renderer createRenderer()
        {
            if(character != null)
                character.close();
            character = new Character(isPreview());
            return new CharacterRenderer(character);
        }

        @Override
        public void onDestroy()
        {
            if(character != null)
                character.close();
            super.onDestroy();
        }

        class CharacterRenderer implements GLSurfaceView.Renderer
        {
            private final String DEFAULT_MODEL = "assets/models/R2-D2/R2-D2.dae";
            private String currentModel;
            private Character character;

            CharacterRenderer(Character character)
            {
                this.character = character;
            }

            @Override
            public void onSurfaceCreated(GL10 gl, EGLConfig config)
            {
            }

            @Override
            public void onSurfaceChanged(GL10 gl, int w, int h)
            {
                character.startup(w, h);
                loadModel();
            }

            @Override
            public void onDrawFrame(GL10 gl)
            {
                character.render();
            }

            private void loadModel()
            {
                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(
                        getApplicationContext());
                final String model = prefs.getString(getString(R.string.prefs_model_key),
                        DEFAULT_MODEL);
                if(model.equals(currentModel) == false)
                {
                    currentModel = model;
                    character.model(model);
                }
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
