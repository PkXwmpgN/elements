package com.yegorov.alexey.preferences;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Parcel;
import android.os.Parcelable;
import android.preference.Preference;
import android.util.AttributeSet;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;


public class PreferenceColorPicker extends Preference
        implements Preference.OnPreferenceClickListener,
        ColorPickerDialog.OnColorChangedListener
{

    View mView;
    ColorPickerDialog mDialog;
    private int mValue = Color.BLACK;
    private float mDensity = 0;
    private boolean mAlphaSliderEnabled = false;

    public PreferenceColorPicker(Context context)
    {
        super(context);
        init(context, null);
    }

    public PreferenceColorPicker(Context context, AttributeSet attrs)
    {
        super(context, attrs);
        init(context, attrs);
    }

    public PreferenceColorPicker(Context context, AttributeSet attrs, int defStyle)
    {
        super(context, attrs, defStyle);
        init(context, attrs);
    }

    @Override
    protected Object onGetDefaultValue(TypedArray a, int index)
    {
        int colorInt;
        String mHexDefaultValue = a.getString(index);
        if(mHexDefaultValue != null && mHexDefaultValue.startsWith("#"))
        {
            colorInt = convertToColorInt(mHexDefaultValue);
            return colorInt;

        }
        else
        {
            return a.getColor(index, Color.BLACK);
        }
    }

    @Override
    protected void onSetInitialValue(boolean restoreValue, Object defaultValue)
    {
        onColorChanged(restoreValue ? getPersistedInt(mValue) : (Integer) defaultValue);
    }

    private void init(Context context, AttributeSet attrs)
    {
        mDensity = getContext().getResources().getDisplayMetrics().density;
        setOnPreferenceClickListener(this);
        if (attrs != null)
        {
            mAlphaSliderEnabled = attrs.getAttributeBooleanValue(null, "alphaSlider", false);
        }
    }

    @Override
    protected void onBindView(View view)
    {
        super.onBindView(view);
        mView = view;
        setPreviewColor();
    }

    private void setPreviewColor()
    {
        if(mView == null)
            return;
        ImageView iView = new ImageView(getContext());
        LinearLayout widgetFrameView = ((LinearLayout) mView.findViewById(android.R.id.widget_frame));
        if(widgetFrameView == null)
            return;
        widgetFrameView.setVisibility(View.VISIBLE);

        int count = widgetFrameView.getChildCount();
        if(count > 0)
            widgetFrameView.removeViews(0, count);

        widgetFrameView.addView(iView);
        widgetFrameView.setMinimumWidth(0);
        iView.setBackgroundDrawable(new AlphaPatternDrawable((int) (5 * mDensity)));
        iView.setImageBitmap(getPreviewBitmap());
    }

    private Bitmap getPreviewBitmap()
    {
        int d = (int) (mDensity * 50);
        int color = mValue;
        Bitmap bm = Bitmap.createBitmap(d, d, Config.ARGB_8888);
        int w = bm.getWidth();
        int h = bm.getHeight();
        for(int i = 0; i < w; i++)
        {
            for(int j = i; j < h; j++)
            {
                int c = (i <= 1 || j <= 1 || i >= w - 2 || j >= h - 2) ? Color.GRAY : color;
                bm.setPixel(i, j, c);
                if(i != j)
                    bm.setPixel(j, i, c);
            }
        }

        return bm;
    }

    @Override
    public void onColorChanged(int color)
    {
        if (isPersistent())
            persistInt(color);

        if(callChangeListener(color))
        {
            mValue = color;
            setPreviewColor();
        }
    }

    public boolean onPreferenceClick(Preference preference)
    {
        showDialog(null);
        return false;
    }

    protected void showDialog(Bundle state)
    {
        mDialog = new ColorPickerDialog(getContext(), mValue);
        mDialog.setOnColorChangedListener(this);

        if(mAlphaSliderEnabled)
            mDialog.setAlphaSliderVisible(true);

        if(state != null)
            mDialog.onRestoreInstanceState(state);

        mDialog.show();
    }

    public void setAlphaSliderEnabled(boolean enable)
    {
        mAlphaSliderEnabled = enable;
    }

    public static String convertToARGB(int color)
    {
        String alpha = Integer.toHexString(Color.alpha(color));
        String red = Integer.toHexString(Color.red(color));
        String green = Integer.toHexString(Color.green(color));
        String blue = Integer.toHexString(Color.blue(color));

        if(alpha.length() == 1)
            alpha = "0" + alpha;

        if(red.length() == 1)
            red = "0" + red;

        if(green.length() == 1)
            green = "0" + green;

        if(blue.length() == 1)
            blue = "0" + blue;

        return "#" + alpha + red + green + blue;
    }

    public static String convertToRGB(int color)
    {
        String red = Integer.toHexString(Color.red(color));
        String green = Integer.toHexString(Color.green(color));
        String blue = Integer.toHexString(Color.blue(color));

        if(red.length() == 1)
            red = "0" + red;

        if(green.length() == 1)
            green = "0" + green;

        if(blue.length() == 1)
            blue = "0" + blue;

        return "#" + red + green + blue;
    }

    public static int convertToColorInt(String argb) throws IllegalArgumentException
    {
        if(!argb.startsWith("#"))
            argb = "#" + argb;

        return Color.parseColor(argb);
    }

    @Override
    protected Parcelable onSaveInstanceState()
    {
        final Parcelable superState = super.onSaveInstanceState();
        if(mDialog == null || !mDialog.isShowing())
            return superState;

        final SavedState myState = new SavedState(superState);
        myState.dialogBundle = mDialog.onSaveInstanceState();
        return myState;
    }

    @Override
    protected void onRestoreInstanceState(Parcelable state)
    {
        if(state == null || !(state instanceof SavedState))
        {
            super.onRestoreInstanceState(state);
            return;
        }

        SavedState myState = (SavedState)state;
        super.onRestoreInstanceState(myState.getSuperState());
        showDialog(myState.dialogBundle);
    }

    private static class SavedState extends BaseSavedState
    {
        Bundle dialogBundle;

        public SavedState(Parcel source)
        {
            super(source);
            dialogBundle = source.readBundle();
        }

        public SavedState(Parcelable superState)
        {
            super(superState);
        }

        @Override
        public void writeToParcel(Parcel dest, int flags)
        {
            super.writeToParcel(dest, flags);
            dest.writeBundle(dialogBundle);
        }

    }
}