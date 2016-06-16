
package com.yegorov.alexey.preferences;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.View;

public class ColorPickerPanelView extends View
{
    private float mDensity = 1f;

    private int mBorderColor = 0xff6E6E6E;
    private int mColor = 0xff000000;

    private Paint mBorderPaint;
    private Paint mColorPaint;

    private RectF mDrawingRect;

    private AlphaPatternDrawable mAlphaPattern;

    public ColorPickerPanelView(Context context)
    {
        this(context, null);
    }

    public ColorPickerPanelView(Context context, AttributeSet attrs)
    {
        this(context, attrs, 0);
    }

    public ColorPickerPanelView(Context context, AttributeSet attrs, int defStyle)
    {
        super(context, attrs, defStyle);
        init();
    }

    private void init()
    {
        mDensity = getContext().getResources().getDisplayMetrics().density;

        mBorderPaint = new Paint();
        mBorderPaint.setColor(mBorderColor);
        mBorderPaint.setStyle(Paint.Style.STROKE);
        mBorderPaint.setStrokeWidth(1.0f);
        mBorderPaint.setAntiAlias(true);

        mColorPaint = new Paint();
    }


    @Override
    protected void onDraw(Canvas canvas)
    {
        if (mAlphaPattern != null)
            mAlphaPattern.draw(canvas);

        mColorPaint.setColor(mColor);

        canvas.drawRect(mDrawingRect, mColorPaint);
        canvas.drawRect(mDrawingRect, mBorderPaint);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec)
    {

        int width = MeasureSpec.getSize(widthMeasureSpec);
        int height = MeasureSpec.getSize(heightMeasureSpec);

        setMeasuredDimension(width, height);
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh)
    {
        super.onSizeChanged(w, h, oldw, oldh);

        mDrawingRect = new RectF();
        mDrawingRect.left = getPaddingLeft();
        mDrawingRect.right = w - getPaddingRight();
        mDrawingRect.top = getPaddingTop();
        mDrawingRect.bottom = h - getPaddingBottom();

        setUpColorRect();

    }

    private void setUpColorRect()
    {
        mAlphaPattern = new AlphaPatternDrawable((int) (5 * mDensity));

        mAlphaPattern.setBounds(
                Math.round(mDrawingRect.left),
                Math.round(mDrawingRect.top),
                Math.round(mDrawingRect.right),
                Math.round(mDrawingRect.bottom)
        );

    }

    public void setColor(int color)
    {
        mColor = color;
        invalidate();
    }

    public int getColor()
    {
        return mColor;
    }

}