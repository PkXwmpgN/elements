package com.yegorov.alexey.preferences;

import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.preference.Preference;
import android.util.AttributeSet;
import android.util.Log;

public class PreferenceRate extends Preference
{
    private static final String TAG = "PreferenceRate";

    public PreferenceRate(Context context)
    {
        super(context);
    }

    public PreferenceRate(Context context, AttributeSet attrs)
    {
        super(context, attrs);
    }

    public PreferenceRate(Context context, AttributeSet attrs, int defStyle)
    {
        super(context, attrs, defStyle);
    }

    @Override
    protected void onClick()
    {
        Uri uri = Uri.parse("market://details?id=" + getContext().getPackageName());
        Intent goToMarket = new Intent(Intent.ACTION_VIEW, uri);
        goToMarket.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY |
                Intent.FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET |
                Intent.FLAG_ACTIVITY_MULTIPLE_TASK);
        try
        {
            getContext().startActivity(goToMarket);
        }
        catch (ActivityNotFoundException e)
        {
            try
            {
                getContext().startActivity(new Intent(Intent.ACTION_VIEW,
                        Uri.parse("http://play.google.com/store/apps/details?id=" +
                                getContext().getPackageName())));
            }
            catch(ActivityNotFoundException ex)
            {
                Log.e(TAG, "[onClick] can't start activity");
            }
        }
    }
}
