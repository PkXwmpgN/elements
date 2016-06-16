package com.yegorov.alexey.preferences;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;

public class ImageTools
{
    static private final String TAG = "ImageTools";

    public static int calculateInSampleSize(BitmapFactory.Options options,
                                            int reqWidth, int reqHeight)
    {
        final int height = options.outHeight;
        final int width = options.outWidth;
        int inSampleSize = 1;

        if (height > reqHeight || width > reqWidth)
        {
            final int halfHeight = height / 2;
            final int halfWidth = width / 2;

            while ((halfHeight / inSampleSize) > reqHeight &&
                    (halfWidth / inSampleSize) > reqWidth)
            {
                inSampleSize *= 2;
            }
        }

        return inSampleSize;
    }

    static public Bitmap decodeSampleBitmap(String path, int reqSize)
    {
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inJustDecodeBounds = true;
        BitmapFactory.decodeFile(path, options);

        options.inJustDecodeBounds = false;
        options.inSampleSize = ImageTools.calculateInSampleSize(options, reqSize, reqSize);
        Bitmap sampleBitmap = BitmapFactory.decodeFile(path, options);
        if(sampleBitmap == null)
        {
            Log.e(TAG, "[decodeSampleBitmap] can't decode sample bitmap");
            return null;
        }

        return squareBitmap(sampleBitmap);
    }

    static public Bitmap decodeSampleAsset(AssetManager assetsManager, String asset, int reqSize)
    {
        try
        {
            InputStream stream = assetsManager.open(asset);

            BitmapFactory.Options options = new BitmapFactory.Options();
            options.inJustDecodeBounds = true;
            BitmapFactory.decodeStream(stream, null, options);
            stream.close();

            stream = assetsManager.open(asset);
            options.inJustDecodeBounds = false;
            options.inSampleSize = ImageTools.calculateInSampleSize(options, reqSize, reqSize);
            Bitmap sampleBitmap = BitmapFactory.decodeStream(stream, null, options);
            stream.close();

            if(sampleBitmap == null)
            {
                Log.e(TAG, "[decodeSampleAsset] can't decode sample asset " + asset);
                return null;
            }

            return squareBitmap(sampleBitmap);
        }
        catch(IOException ex)
        {
            Log.e(TAG, "[backgroundTexture] can't load default asset");
        }

        return null;
    }

    static private Bitmap squareBitmap(Bitmap bitmap)
    {
        if(bitmap.getWidth() != bitmap.getHeight())
        {
            final int size = Math.min(bitmap.getWidth(), bitmap.getHeight());
            final int x = (bitmap.getWidth() - size) / 2;
            final int y = (bitmap.getHeight() - size) / 2;

            return Bitmap.createBitmap(bitmap, x, y, size, size);
        }

        return bitmap;
    }
}
