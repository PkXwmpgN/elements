package com.yegorov.alexey.elements.character;

import com.yegorov.alexey.preferences.PreferenceActivity;
import com.yegorov.alexey.preferences.PreferenceFragment;

import android.os.Bundle;
import android.support.v7.app.ActionBar;
import android.view.MenuItem;


public class CharacterWallpaperSettings extends PreferenceActivity
{
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        ActionBar actionBar = getSupportActionBar();
        if(actionBar != null)
        {
            actionBar.setDisplayHomeAsUpEnabled(true);
        }

        getFragmentManager().beginTransaction().replace(android.R.id.content,
                new StrangePreferenceFragment()).commit();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        switch(item.getItemId())
        {
            case android.R.id.home:
                onBackPressed();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    public static class StrangePreferenceFragment extends PreferenceFragment
    {
        @Override
        public void onCreate(final Bundle savedInstanceState)
        {
            super.onCreate(savedInstanceState);
            addPreferencesFromResource(R.xml.preferences);
        }
    }

}
