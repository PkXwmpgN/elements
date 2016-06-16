package com.yegorov.alexey.preferences;

import android.preference.PreferenceManager.OnActivityResultListener;
import android.preference.Preference;
import android.content.Intent;
import android.preference.PreferenceGroup;

public class PreferenceFragment extends android.preference.PreferenceFragment
{
    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        for(int i = 0; i < getPreferenceScreen().getPreferenceCount(); ++i)
        {
            Preference preference = getPreferenceScreen().getPreference(i);
            if(preference != null)
            {
                if(activityResult(preference, requestCode, resultCode, data))
                    return;
            }
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    private boolean activityResult(Preference preference, int requestCode, int resultCode,
                                   Intent data)
    {
        if(preference instanceof PreferenceGroup)
        {
            PreferenceGroup group = (PreferenceGroup)preference;
            if(group != null)
            {
                for(int i = 0; i < group.getPreferenceCount(); ++i)
                {
                    if(activityResult(group.getPreference(i), requestCode, resultCode, data))
                        return true;
                }
            }
        }
        else if(preference instanceof OnActivityResultListener)
        {
            return ((OnActivityResultListener) preference).onActivityResult(requestCode,
                    resultCode, data);
        }
        return false;
    }
}
