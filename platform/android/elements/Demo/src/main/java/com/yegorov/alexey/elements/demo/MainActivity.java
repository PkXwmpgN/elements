package com.yegorov.alexey.elements.demo;

import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.WindowManager;

import com.yegorov.alexey.elements.api.Elements;
import com.yegorov.alexey.elements.utils.SensorInterpreter;

public class MainActivity extends Activity
      implements SensorEventListener
{
    private MainSurface surface;
    private Demo demo;
    private SensorInterpreter sensorInterpreter;
    private SensorManager sensors;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        sensors = (SensorManager)getSystemService(Context.SENSOR_SERVICE);
        sensorInterpreter = new SensorInterpreter();

        demo = new Demo();

        surface = new MainSurface(this, demo);
        surface.setEGLConfigChooser(8, 8, 8, 8, 24, 8);
        surface.setEGLContextClientVersion(2);
        surface.setPreserveEGLContextOnPause(true);
        surface.setRenderer(new MainRender(demo));
        setContentView(surface);

        WindowManager wm = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
        DisplayMetrics metrics = new DisplayMetrics();
        wm.getDefaultDisplay().getMetrics(metrics);

        Elements.initializeAssets(getAssets());
        Elements.initializeMetrics(metrics);
    }

    @Override
    protected void onDestroy()
    {
        demo.close();
        super.onDestroy();
    }

    @Override
    protected void onPause()
    {
        super.onPause();
        if(surface != null)
            surface.onPause();

        sensors.unregisterListener(this);
    }

    @Override
    protected void onResume()
    {
        super.onResume();

        if(surface != null)
            surface.onResume();

        sensors.registerListener(this, sensors.getDefaultSensor(Sensor.TYPE_GAME_ROTATION_VECTOR),
                SensorManager.SENSOR_DELAY_FASTEST);

        sensorInterpreter.reset();
    }

    @Override
    public void onSensorChanged(SensorEvent event)
    {
        if(demo == null)
            return;

        if(event.sensor.getType() == Sensor.TYPE_GAME_ROTATION_VECTOR)
        {
            float [] delta = sensorInterpreter.rotation(getApplicationContext(), event);
            if(delta != null)
            {
                demo.rotation(delta[0], delta[1], delta[2]);
            }
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy)
    {

    }
}
