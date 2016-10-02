package com.yegorov.alexey.elements.demo;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView.Renderer;

public class MainRender implements Renderer
{
    private Demo demo;

    public MainRender(Demo demo)
    {
        this.demo = demo;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        demo.startup(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {
        demo.render();
    }
}
