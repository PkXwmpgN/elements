package com.yegorov.alexey.elements.liquid;

import com.yegorov.alexey.preferences.PreferenceActivity;
import com.yegorov.alexey.preferences.PreferenceFragment;

import android.Manifest;
import android.content.DialogInterface;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.preference.Preference;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AlertDialog;
import android.view.MenuItem;


public class LiquidWallpaperSettings extends PreferenceActivity
{
    private static final int PERMISSIONS_REQUEST_EXTERNAL_STORAGE = 0;

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
                new LiquidPreferenceFragment()).commit();
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
    
    public static class LiquidPreferenceFragment extends PreferenceFragment
    {
        @Override
        public void onCreate(final Bundle savedInstanceState)
        {
            super.onCreate(savedInstanceState);
            addPreferencesFromResource(R.xml.preferences);
        }

        @Override
        public void onStart()
        {
            super.onStart();

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M)
            {
                final int checkRead = ContextCompat.checkSelfPermission(getActivity(),
                        Manifest.permission.READ_EXTERNAL_STORAGE);
                final int checkWrite = ContextCompat.checkSelfPermission(getActivity(),
                        Manifest.permission.WRITE_EXTERNAL_STORAGE);

                if (checkRead != PackageManager.PERMISSION_GRANTED ||
                    checkWrite != PackageManager.PERMISSION_GRANTED)
                {
                    requestPermissions(new String[]
                    {
                            Manifest.permission.READ_EXTERNAL_STORAGE,
                            Manifest.permission.WRITE_EXTERNAL_STORAGE
                    }, PERMISSIONS_REQUEST_EXTERNAL_STORAGE);
                }
            }
        }

        @Override
        public void onRequestPermissionsResult(int requestCode,
                                               String permissions[],
                                               int[] grantResults)
        {
            if(requestCode == PERMISSIONS_REQUEST_EXTERNAL_STORAGE)
            {
                if(grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED)
                {
                    final String key = getString(R.string.prefs_background_key);
                    Preference background = getPreferenceScreen().findPreference(key);
                    if(background != null)
                        background.setOnPreferenceClickListener(null);
                }
                else
                {
                    final String key = getString(R.string.prefs_background_key);
                    Preference background = getPreferenceScreen().findPreference(key);
                    if(background != null)
                    {
                        background.setOnPreferenceClickListener(new Preference.OnPreferenceClickListener()
                        {
                            @Override
                            public boolean onPreferenceClick(Preference preference)
                            {
                                AlertDialog alertDialog = new AlertDialog.Builder(getActivity()).create();
                                alertDialog.setTitle(getString(R.string.permission_title));
                                alertDialog.setMessage(getString(R.string.permission_text));
                                alertDialog.setButton(AlertDialog.BUTTON_NEUTRAL, "OK",
                                        new DialogInterface.OnClickListener()
                                        {
                                            public void onClick(DialogInterface dialog, int which)
                                            {
                                                dialog.dismiss();
                                            }
                                        });
                                alertDialog.show();
                                return true;
                            }
                        });
                    }
                }
            }
        }
    }

}
