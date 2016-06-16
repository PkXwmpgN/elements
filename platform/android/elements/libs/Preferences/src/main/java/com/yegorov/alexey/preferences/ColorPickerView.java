
package com.yegorov.alexey.preferences;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.ComposeShader;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Point;
import android.graphics.PorterDuff;
import android.graphics.RectF;
import android.graphics.Shader;
import android.graphics.Shader.TileMode;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

public class ColorPickerView extends View
{

    private final static int PANEL_SAT_VAL = 0;
    private final static int PANEL_HUE = 1;
    private final static int PANEL_ALPHA = 2;

    private float HUE_PANEL_WIDTH = 20f;
    private float ALPHA_PANEL_HEIGHT = 20f;
    private float PANEL_SPACING = 4.0f;
    private float TRACKER_RADIUS = 6;

    private float mDensity = 1.0f;

    private OnColorChangedListener mListener;

    private Paint mSatValPaint;
    private Paint mHuePaint;
    private Paint mAlphaPaint;

    private Paint mTrackerPaint;
    private Paint mBorderPaint;

    private Shader mValShader;
    private Shader mSatShader;
    private Shader mHueShader;
    private Shader mAlphaShader;

    private int mAlpha = 0xff;
    private float mHue = 360f;
    private float mSat = 0f;
    private float mVal = 0f;

    private int mBorderColor = 0xff6E6E6E;
    private int mColorBackground = 0xff37474F;
    private boolean mShowAlphaPanel = false;

    /*
     * To remember which panel that has the "focus" when
     * processing hardware button data.
     */
    private int mLastTouchedPanel = PANEL_SAT_VAL;

    /**
     * Offset from the edge we must have or else
     * the finger tracker will get clipped when
     * it is drawn outside of the view.
     */
    private float mDrawingOffset;


    /*
     * Distance form the edges of the view
     * of where we are allowed to draw.
     */
    private RectF mDrawingRect;

    private RectF mSatValRect;
    private RectF mHueRect;
    private RectF mAlphaRect;

    private AlphaPatternDrawable mAlphaPattern;

    private Point mStartTouchPoint = null;

    public interface OnColorChangedListener
    {
        public void onColorChanged(int color);
    }

    public ColorPickerView(Context context)
    {
        this(context, null);
    }

    public ColorPickerView(Context context, AttributeSet attrs)
    {
        this(context, attrs, 0);
    }

    public ColorPickerView(Context context, AttributeSet attrs, int defStyle)
    {
        super(context, attrs, defStyle);
        init();
    }

    private void init()
    {
        mDensity = getContext().getResources().getDisplayMetrics().density;

        TRACKER_RADIUS *= mDensity;
        HUE_PANEL_WIDTH *= mDensity;
        ALPHA_PANEL_HEIGHT *= mDensity;
        PANEL_SPACING = PANEL_SPACING * mDensity;

        mDrawingOffset = calculateRequiredOffset();

        initPaintTools();

        setFocusable(true);
        setFocusableInTouchMode(true);
    }

    private void initPaintTools()
    {
        mSatValPaint = new Paint();
        mHuePaint = new Paint();
        mTrackerPaint = new Paint();
        mAlphaPaint = new Paint();
        mBorderPaint = new Paint();

        mBorderPaint.setColor(mBorderColor);
        mBorderPaint.setStyle(Style.STROKE);
        mBorderPaint.setStrokeWidth(1.0f);
        mBorderPaint.setAntiAlias(true);

        mTrackerPaint.setStyle(Style.FILL);
    }

    private float calculateRequiredOffset()
    {
        return Math.max(TRACKER_RADIUS, 1.0f * mDensity) * 1.5f;
    }

    private int[] buildHueColorArray()
    {
        int[] hue = new int[361];

        int count = 0;
        for (int i = hue.length - 1; i >= 0; i--, count++)
            hue[count] = Color.HSVToColor(new float[]{i, 1f, 1f});

        return hue;
    }


    @Override
    protected void onDraw(Canvas canvas)
    {
        if (mDrawingRect.width() <= 0 || mDrawingRect.height() <= 0)
            return;

        drawSatValPanel(canvas);
        drawHuePanel(canvas);
        drawAlphaPanel(canvas);
    }

    private void drawSatValPanel(Canvas canvas)
    {
        final RectF rect = mSatValRect;

        if (mValShader == null)
        {
            mValShader = new LinearGradient(rect.left, rect.top, rect.left, rect.bottom,
                    0xffffffff, 0xff000000, TileMode.CLAMP);
        }

        int rgb = Color.HSVToColor(new float[]{mHue, 1f, 1f});

        mSatShader = new LinearGradient(rect.left, rect.top, rect.right, rect.top,
                0xffffffff, rgb, TileMode.CLAMP);
        ComposeShader mShader = new ComposeShader(mValShader, mSatShader, PorterDuff.Mode.MULTIPLY);
        mSatValPaint.setShader(mShader);

        canvas.drawRect(rect, mSatValPaint);
        canvas.drawRect(rect, mBorderPaint);

        Point p = satValToPoint(mSat, mVal);
        float offset = TRACKER_RADIUS + 1.0f * mDensity;

        mTrackerPaint.setColor(mColorBackground);
        canvas.drawRect(p.x - offset, p.y - offset,
                p.x + offset, p.y + offset, mTrackerPaint);

        mTrackerPaint.setColor(Color.HSVToColor(255, new float[]{mHue, mSat, mVal}));
        canvas.drawRect(p.x - TRACKER_RADIUS,
                p.y - TRACKER_RADIUS,
                p.x + TRACKER_RADIUS,
                p.y + TRACKER_RADIUS, mTrackerPaint);

        canvas.drawRect(p.x - TRACKER_RADIUS,
                p.y - TRACKER_RADIUS,
                p.x + TRACKER_RADIUS,
                p.y + TRACKER_RADIUS, mBorderPaint);
    }

    private void drawHuePanel(Canvas canvas)
    {
        final float widthOffset = mHueRect.width() / 4;
        final RectF rect = new RectF(mHueRect.left + widthOffset,
                mHueRect.top, mHueRect.right - widthOffset, mHueRect.bottom);

        if (mHueShader == null)
        {
            mHueShader = new LinearGradient(rect.left, rect.top, rect.left, rect.bottom, buildHueColorArray(), null, TileMode.CLAMP);
            mHuePaint.setShader(mHueShader);
        }

        canvas.drawRect(rect, mHuePaint);

        Point p = hueToPoint(mHue);
        float offset = TRACKER_RADIUS + 4.0f * mDensity;

        mTrackerPaint.setColor(mColorBackground);
        canvas.drawRect(rect.left, p.y - offset, rect.right, p.y + offset, mTrackerPaint);

        mTrackerPaint.setColor(Color.HSVToColor(255, new float[]{mHue, 255, 255}));
        canvas.drawRect(p.x - TRACKER_RADIUS,
                p.y - TRACKER_RADIUS,
                p.x + TRACKER_RADIUS,
                p.y + TRACKER_RADIUS, mTrackerPaint);

        canvas.drawRect(p.x - TRACKER_RADIUS,
                p.y - TRACKER_RADIUS,
                p.x + TRACKER_RADIUS,
                p.y + TRACKER_RADIUS, mBorderPaint);

        if(p.y - offset > rect.top)
            canvas.drawRect(rect.left, rect.top, rect.right, p.y - offset, mBorderPaint);
        if(p.y + offset < rect.bottom)
            canvas.drawRect(rect.left, p.y + offset, rect.right, rect.bottom, mBorderPaint);
    }

    private void drawAlphaPanel(Canvas canvas)
    {
        if (!mShowAlphaPanel || mAlphaRect == null || mAlphaPattern == null)
            return;

        final float heightOffset = mAlphaRect.height() / 4;
        final RectF rect = new RectF(mAlphaRect.left, mAlphaRect.top + heightOffset,
                mAlphaRect.right, mAlphaRect.bottom - heightOffset);

        mAlphaPattern.draw(canvas);

        float[] hsv = new float[]{mHue, mSat, mVal};
        int color = Color.HSVToColor(hsv);
        int acolor = Color.HSVToColor(0, hsv);

        mAlphaShader = new LinearGradient(rect.left, rect.top, rect.right, rect.top,
                color, acolor, TileMode.CLAMP);

        mAlphaPaint.setShader(mAlphaShader);
        canvas.drawRect(rect, mAlphaPaint);

        Point p = alphaToPoint(mAlpha);
        if(p.x + TRACKER_RADIUS > mDrawingRect.right - mHueRect.width() / 2)
            p.x = (int)(mDrawingRect.right - mHueRect.width() / 2);

        float offset = TRACKER_RADIUS + 4.0f * mDensity;

        mTrackerPaint.setColor(mColorBackground);
        canvas.drawRect(p.x - offset, rect.top, p.x + offset, rect.bottom, mTrackerPaint);

        mTrackerPaint.setColor(0xffffffff);
        canvas.drawRect(p.x - TRACKER_RADIUS,
                p.y - TRACKER_RADIUS,
                p.x + TRACKER_RADIUS,
                p.y + TRACKER_RADIUS, mTrackerPaint);

        mTrackerPaint.setColor(Color.HSVToColor(mAlpha, new float[]{mHue, mSat, mVal}));
        canvas.drawRect(p.x - TRACKER_RADIUS,
                p.y - TRACKER_RADIUS,
                p.x + TRACKER_RADIUS,
                p.y + TRACKER_RADIUS, mTrackerPaint);

        canvas.drawRect(p.x - TRACKER_RADIUS,
                p.y - TRACKER_RADIUS,
                p.x + TRACKER_RADIUS,
                p.y + TRACKER_RADIUS, mBorderPaint);

        if(p.x - offset > rect.left)
            canvas.drawRect(rect.left, rect.top, p.x - offset, rect.bottom, mBorderPaint);
        if(p.x + offset < rect.right)
            canvas.drawRect(p.x + offset, rect.top, rect.right, rect.bottom, mBorderPaint);
    }

    private Point hueToPoint(float hue)
    {
        final RectF rect = mHueRect;
        final float height = rect.height();

        Point p = new Point();

        p.y = (int) (height - (hue * height / 360f) + rect.top);
        p.x = (int) rect.centerX();

        return p;
    }

    private Point satValToPoint(float sat, float val)
    {
        final RectF rect = mSatValRect;
        final float height = rect.height();
        final float width = rect.width();

        Point p = new Point();

        p.x = (int) (sat * width + rect.left);
        p.y = (int) ((1f - val) * height + rect.top);

        return p;
    }

    private Point alphaToPoint(int alpha)
    {
        final RectF rect = mAlphaRect;
        final float width = rect.width();

        Point p = new Point();

        p.x = (int) (width - (alpha * width / 0xff) + rect.left);
        p.y = (int) rect.centerY();

        return p;
    }

    private float[] pointToSatVal(float x, float y)
    {
        final RectF rect = mSatValRect;
        float[] result = new float[2];

        float width = rect.width();
        float height = rect.height();

        if (x < rect.left)
            x = 0f;
        else if (x > rect.right)
            x = width;
        else
            x = x - rect.left;

        if (y < rect.top)
            y = 0f;
        else if (y > rect.bottom)
            y = height;
        else
            y = y - rect.top;

        result[0] = 1.f / width * x;
        result[1] = 1.f - (1.f / height * y);

        return result;
    }

    private float pointToHue(float y)
    {
        final RectF rect = mHueRect;
        float height = rect.height();

        if (y < rect.top)
            y = 0f;
        else if (y > rect.bottom)
            y = height;
        else
            y = y - rect.top;

        return 360f - (y * 360f / height);
    }

    private int pointToAlpha(int x)
    {
        final RectF rect = mAlphaRect;
        final int width = (int) rect.width();

        if (x < rect.left)
            x = 0;
        else if (x > rect.right)
            x = width;
        else
            x = x - (int) rect.left;

        return 0xff - (x * 0xff / width);
    }

    @Override
    public boolean onTrackballEvent(MotionEvent event)
    {
        float x = event.getX();
        float y = event.getY();

        boolean update = false;

        if (event.getAction() == MotionEvent.ACTION_MOVE)
        {
            switch (mLastTouchedPanel)
            {
                case PANEL_SAT_VAL:

                    float sat, val;

                    sat = mSat + x / 50f;
                    val = mVal - y / 50f;

                    if (sat < 0f) {
                        sat = 0f;
                    } else if (sat > 1f) {
                        sat = 1f;
                    }

                    if (val < 0f) {
                        val = 0f;
                    } else if (val > 1f) {
                        val = 1f;
                    }

                    mSat = sat;
                    mVal = val;

                    update = true;

                    break;

                case PANEL_HUE:

                    float hue = mHue - y * 10f;

                    if (hue < 0f) {
                        hue = 0f;
                    } else if (hue > 360f) {
                        hue = 360f;
                    }

                    mHue = hue;

                    update = true;

                    break;

                case PANEL_ALPHA:

                    if (!mShowAlphaPanel || mAlphaRect == null) {
                        update = false;
                    } else {

                        int alpha = (int) (mAlpha - x * 10);

                        if (alpha < 0) {
                            alpha = 0;
                        } else if (alpha > 0xff) {
                            alpha = 0xff;
                        }

                        mAlpha = alpha;


                        update = true;
                    }
                    break;
            }
        }

        if (update)
        {
            if (mListener != null)
                mListener.onColorChanged(Color.HSVToColor(mAlpha, new float[]{mHue, mSat, mVal}));

            invalidate();
            return true;
        }

        return super.onTrackballEvent(event);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
        boolean update = false;
        switch (event.getAction())
        {
            case MotionEvent.ACTION_DOWN:
                mStartTouchPoint = new Point((int) event.getX(), (int) event.getY());
                update = moveTrackersIfNeeded(event);
                break;

            case MotionEvent.ACTION_MOVE:
                update = moveTrackersIfNeeded(event);
                break;

            case MotionEvent.ACTION_UP:
                mStartTouchPoint = null;
                update = moveTrackersIfNeeded(event);
                break;
        }

        if (update)
        {
            if (mListener != null)
                mListener.onColorChanged(Color.HSVToColor(mAlpha, new float[]{mHue, mSat, mVal}));

            invalidate();
            return true;
        }

        return super.onTouchEvent(event);
    }

    private boolean moveTrackersIfNeeded(MotionEvent event)
    {
        if (mStartTouchPoint == null)
            return false;

        boolean update = false;

        int startX = mStartTouchPoint.x;
        int startY = mStartTouchPoint.y;

        if (mHueRect.contains(startX, startY))
        {
            mLastTouchedPanel = PANEL_HUE;
            mHue = pointToHue(event.getY());
            update = true;
        }
        else if (mSatValRect.contains(startX, startY))
        {
            mLastTouchedPanel = PANEL_SAT_VAL;

            float[] result = pointToSatVal(event.getX(), event.getY());

            mSat = result[0];
            mVal = result[1];

            update = true;
        }
        else if (mAlphaRect != null && mAlphaRect.contains(startX, startY))
        {
            mLastTouchedPanel = PANEL_ALPHA;
            mAlpha = pointToAlpha((int) event.getX());
            update = true;
        }

        return update;
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec)
    {
        int width = 0;
        int height = 0;

        int widthMode = MeasureSpec.getMode(widthMeasureSpec);
        int heightMode = MeasureSpec.getMode(heightMeasureSpec);

        int widthAllowed = MeasureSpec.getSize(widthMeasureSpec);
        int heightAllowed = MeasureSpec.getSize(heightMeasureSpec);

        widthAllowed = chooseWidth(widthMode, widthAllowed);
        heightAllowed = chooseHeight(heightMode, heightAllowed);

        if (!mShowAlphaPanel)
        {
            height = (int) (widthAllowed - PANEL_SPACING - HUE_PANEL_WIDTH);

            if (height > heightAllowed)
            {
                height = heightAllowed;
                width = (int) (height + PANEL_SPACING + HUE_PANEL_WIDTH);
            }
            else
            {
                width = widthAllowed;
            }
        }
        else
        {
            width = (int) (heightAllowed - ALPHA_PANEL_HEIGHT + HUE_PANEL_WIDTH);
            if (width > widthAllowed)
            {
                width = widthAllowed;
                height = (int) (widthAllowed - HUE_PANEL_WIDTH + ALPHA_PANEL_HEIGHT);
            }
            else
            {
                height = heightAllowed;
            }
        }

        setMeasuredDimension(width, height);
    }

    private int chooseWidth(int mode, int size)
    {
        if (mode == MeasureSpec.AT_MOST || mode == MeasureSpec.EXACTLY)
            return size;
        else // (mode == MeasureSpec.UNSPECIFIED)
            return getPrefferedWidth();
    }

    private int chooseHeight(int mode, int size)
    {
        if (mode == MeasureSpec.AT_MOST || mode == MeasureSpec.EXACTLY)
            return size;
        else// (mode == MeasureSpec.UNSPECIFIED)
            return getPrefferedHeight();
    }

    private int getPrefferedWidth()
    {
        int width = getPrefferedHeight();
        if (mShowAlphaPanel)
            width -= (PANEL_SPACING + ALPHA_PANEL_HEIGHT);

        return (int) (width + HUE_PANEL_WIDTH + PANEL_SPACING);

    }

    private int getPrefferedHeight()
    {
        int height = (int) (200 * mDensity);
        if (mShowAlphaPanel)
            height += PANEL_SPACING + ALPHA_PANEL_HEIGHT;

        return height;
    }


    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh)
    {
        super.onSizeChanged(w, h, oldw, oldh);

        mDrawingRect = new RectF();
        mDrawingRect.left = mDrawingOffset + getPaddingLeft();
        mDrawingRect.right = w - mDrawingOffset - getPaddingRight();
        mDrawingRect.top = mDrawingOffset + getPaddingTop();
        mDrawingRect.bottom = h - mDrawingOffset - getPaddingBottom();

        setUpSatValRect();
        setUpHueRect();
        setUpAlphaRect();
    }

    private void setUpSatValRect()
    {
        final RectF dRect = mDrawingRect;
        float panelSide = dRect.height();

        if (mShowAlphaPanel)
            panelSide -= PANEL_SPACING + ALPHA_PANEL_HEIGHT;

        float left = dRect.left;
        float top = dRect.top;
        float bottom = top + panelSide;
        float right = left + panelSide;

        mSatValRect = new RectF(left, top, right, bottom);
    }

    private void setUpHueRect()
    {
        final RectF dRect = mDrawingRect;

        float left = dRect.right - HUE_PANEL_WIDTH;
        float top = dRect.top;
        float bottom = dRect.bottom - (mShowAlphaPanel ? (PANEL_SPACING + ALPHA_PANEL_HEIGHT) : 0);
        float right = dRect.right;

        mHueRect = new RectF(left, top, right, bottom);
    }

    private void setUpAlphaRect()
    {
        if (!mShowAlphaPanel) return;

        final RectF dRect = mDrawingRect;

        float left = dRect.left;
        float top = dRect.bottom - ALPHA_PANEL_HEIGHT;
        float bottom = dRect.bottom;
        float right = dRect.right - HUE_PANEL_WIDTH / 4;

        mAlphaRect = new RectF(left, top, right, bottom);

        final float offset = mAlphaRect.height() / 4;
        mAlphaPattern = new AlphaPatternDrawable((int) (5 * mDensity));
        mAlphaPattern.setBounds(
                Math.round(mAlphaRect.left),
                Math.round(mAlphaRect.top + offset),
                Math.round(mAlphaRect.right),
                Math.round(mAlphaRect.bottom - offset)
        );

    }


    /**
     * Set a OnColorChangedListener to get notified when the color
     * selected by the user has changed.
     *
     * @param listener
     */
    public void setOnColorChangedListener(OnColorChangedListener listener)
    {
        mListener = listener;
    }

    /**
     * Get the current color this view is showing.
     *
     * @return the current color.
     */
    public int getColor()
    {
        return Color.HSVToColor(mAlpha, new float[]{mHue, mSat, mVal});
    }

    /**
     * Set the color the view should show.
     *
     * @param color The color that should be selected.
     */
    public void setColor(int color)
    {
        setColor(color, false);
    }

    /**
     * Set the color this view should show.
     *
     * @param color    The color that should be selected.
     * @param callback If you want to get a callback to
     *                 your OnColorChangedListener.
     */
    public void setColor(int color, boolean callback)
    {
        int alpha = Color.alpha(color);

        float[] hsv = new float[3];

        Color.colorToHSV(color, hsv);

        mAlpha = alpha;
        mHue = hsv[0];
        mSat = hsv[1];
        mVal = hsv[2];

        if (callback && mListener != null)
            mListener.onColorChanged(Color.HSVToColor(mAlpha, new float[]{mHue, mSat, mVal}));

        invalidate();
    }

    /**
     * Get the drawing offset of the color picker view.
     * The drawing offset is the distance from the side of
     * a panel to the side of the view minus the padding.
     * Useful if you want to have your own panel below showing
     * the currently selected color and want to align it perfectly.
     *
     * @return The offset in pixels.
     */
    public float getDrawingOffset()
    {
        return mDrawingOffset;
    }

    /**
     * Set if the user is allowed to adjust the alpha panel. Default is false.
     * If it is set to false no alpha will be set.
     *
     * @param visible
     */
    public void setAlphaSliderVisible(boolean visible)
    {
        if (mShowAlphaPanel != visible)
        {
            mShowAlphaPanel = visible;

            mValShader = null;
            mSatShader = null;
            mHueShader = null;
            mAlphaShader = null;

            requestLayout();
        }
    }

    public boolean getAlphaSliderVisible()
    {
        return mShowAlphaPanel;
    }
}