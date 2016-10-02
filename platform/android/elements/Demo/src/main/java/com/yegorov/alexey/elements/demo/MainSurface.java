package com.yegorov.alexey.elements.demo;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.support.v4.view.MotionEventCompat;
import android.view.MotionEvent;

public class MainSurface extends GLSurfaceView
{
    Demo demo;

    public MainSurface(Context context, Demo demo)
    {
        super(context);
        this.demo = demo;
    }


    @Override
    public boolean onTouchEvent(final MotionEvent event)
    {
        final int pointerIndex = MotionEventCompat.getActionIndex(event);
        final int pointerAction =  MotionEventCompat.getActionMasked(event);
        return demo.touch(event.getX(pointerIndex),
                          event.getY(pointerIndex),
                          pointerAction,
                          pointerIndex);
    }
}
