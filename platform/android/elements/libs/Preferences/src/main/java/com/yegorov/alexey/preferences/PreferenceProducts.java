package com.yegorov.alexey.preferences;

import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.preference.Preference;
import android.util.AttributeSet;
import android.util.Log;

public class PreferenceProducts extends Preference
{
    private static final String TAG = "PreferenceRate";
    private static final String PUBLISHER_NAME = "Alexey Yegorov";

    public PreferenceProducts(Context context)
    {
        super(context);
    }

    public PreferenceProducts(Context context, AttributeSet attrs)
    {
        super(context, attrs);
    }

    public PreferenceProducts(Context context, AttributeSet attrs, int defStyle)
    {
        super(context, attrs, defStyle);
    }

    @Override
    protected void onClick()
    {
        Uri uri = Uri.parse("market://search?q=pub:" + PUBLISHER_NAME);
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
                        Uri.parse("http://play.google.com/store/apps/search?q=pub:" +
                                PUBLISHER_NAME)));
            }
            catch(ActivityNotFoundException ex)
            {
                Log.e(TAG, "[onClick] can't start activity");
            }
        }
    }
}
