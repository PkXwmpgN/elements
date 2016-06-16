package com.yegorov.alexey.preferences;

import android.app.Activity;
import android.app.ProgressDialog;
import android.graphics.Matrix;
import android.net.Uri;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Environment;
import android.preference.Preference;
import android.preference.PreferenceManager;
import android.preference.PreferenceManager.OnActivityResultListener;
import android.provider.MediaStore;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.LinearLayout;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class PreferenceImagePicker extends Preference
        implements OnActivityResultListener
{
    private static final String TAG = "ImagePreferences";
    private static final int CROP_IMAGE = 1;
    private static final int PICK_IMAGE = 2;

    public static final String PREVIEW_SUFFIX   = "_preview";
    public static final String PORTRAIT_SUFFIX  = "_portrait";
    public static final String LANDSCAPE_SUFFIX = "_landscape";

    private static final int PREVIEW_SIZE = 50;

    private View view;

    public PreferenceImagePicker(Context context)
    {
        super(context);
    }

    public PreferenceImagePicker(Context context, AttributeSet attrs)
    {
        super(context, attrs);
    }

    public PreferenceImagePicker(Context context, AttributeSet attrs, int defStyle)
    {
        super(context, attrs, defStyle);
    }

    @Override
    protected void onBindView(View view)
    {
        super.onBindView(view);
        this.view = view;

        updatePreview();
    }

    void updatePreview()
    {
        if(view == null)
        {
            Log.e(TAG, "[updatePreview] view is null");
            return;
        }

        ImageView preView = new ImageView(getContext());
        LinearLayout frame = (LinearLayout)view.findViewById(android.R.id.widget_frame);
        if(frame == null)
        {
            Log.e(TAG, "[updatePreview] can't find view");
            return;
        }

        frame.setVisibility(View.VISIBLE);
        int count = frame.getChildCount();
        if(count > 0)
            frame.removeViews(0, count);

        WindowManager wm = (WindowManager)getContext().getSystemService(Context.WINDOW_SERVICE);
        DisplayMetrics metrics = new DisplayMetrics();
        wm.getDefaultDisplay().getMetrics(metrics);

        final float density = metrics.density;

        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inSampleSize = 8;
        Bitmap bitmap = null;

        File file = ImageWriter.imagePreviewFile(getContext(), getKey());
        if(file.exists())
        {
            bitmap = BitmapFactory.decodeFile(file.getPath(), options);
        }

        if(bitmap == null)
        {
            Log.e(TAG, "[updatePreview] can't create preview.");
            return;
        }

        frame.addView(preView, (int) (density * PREVIEW_SIZE), (int) (density * PREVIEW_SIZE));
        frame.setMinimumWidth(0);

        preView.setImageBitmap(bitmap);
        preView.setPadding(1, 1, 1, 1);

        preView.setBackgroundColor(Color.GRAY);
    }

    private Intent prepareCropIntent()
    {
        try
        {
            Intent intent = new Intent(Intent.ACTION_PICK,
                    android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
            intent.setType("image/*");
            intent.putExtra("crop", "true");
            intent.putExtra("aspectX", 1);
            intent.putExtra("aspectY", 1);
            intent.putExtra("return-data", false);

            File path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES);
            File file = new File(path, getKey());

            path.mkdirs();
            file.createNewFile();

            if(file.exists())
            {
                intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(file));
                return intent;
            }
            else
            {
                Log.e(TAG, "[prepareCropIntent] can't create output file");
            }

            return null;
        }
        catch(IOException ex)
        {
            Log.e(TAG, "[prepareCropIntent] can't create output");
            ex.printStackTrace();
        }

        return null;
    }

    private Intent preparePickIntent()
    {
        Intent intent = new Intent(Intent.ACTION_PICK,
                android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
        intent.setType("image/*");
        return intent;
    }

    private boolean isIntentAvailable(Context ctx, Intent intent)
    {
        final PackageManager manager = ctx.getPackageManager();
        return manager.queryIntentActivities(intent,
                PackageManager.MATCH_DEFAULT_ONLY).size() > 0;
    }

    @Override
    protected void onClick()
    {
        if(getOnPreferenceClickListener() == null)
        {
            Intent intent = prepareCropIntent();
            if (intent != null && isIntentAvailable(getContext(), intent))
                ((Activity) getContext()).startActivityForResult(intent, CROP_IMAGE);
            else
            {
                Log.i(TAG, "[onClick] crop intent is not available");
                ((Activity) getContext()).startActivityForResult(preparePickIntent(), PICK_IMAGE);
            }
        }
    }

    @Override
    public boolean onActivityResult(int requestCode, int resultCode, Intent data)
    {
        if(requestCode == CROP_IMAGE && resultCode == Activity.RESULT_OK)
        {
            File path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES);
            File file = new File(path, getKey());
            if(file.exists() && callChangeListener(null))
            {
                ImageWriter writer = new ImageWriter(getContext(), getKey());
                writer.saveImagesAsync(file.getPath(),
                        new ImageWriter.OnSaveImagesCompleteListener()
                        {
                            @Override
                            public void onComplete() {
                                updatePreview();
                            }
                        }, true);
            }
            else
            {
                Log.e(TAG, "[onActivityResult] output is not exist");
            }

            return true;
        }
        else if(requestCode == PICK_IMAGE && resultCode == Activity.RESULT_OK)
        {
            try
            {
                Uri imageUri = data.getData();
                String[] column = {MediaStore.Images.Media.DATA};

                Cursor cursor = getContext().getContentResolver().query(imageUri, column,
                        null, null, null);
                if(cursor != null)
                {
                    cursor.moveToFirst();

                    int index = cursor.getColumnIndex(column[0]);
                    String path = cursor.getString(index);
                    cursor.close();

                    if(callChangeListener(null))
                    {
                        ImageWriter writer = new ImageWriter(getContext(), getKey());
                        writer.saveImagesAsync(path,
                                new ImageWriter.OnSaveImagesCompleteListener()
                                {
                                    @Override
                                    public void onComplete()
                                    {
                                        updatePreview();
                                    }
                                }, false);
                    }
                }
                else
                {
                    Log.e(TAG, "[onActivityResult] cursor is null");
                }
                return true;
            }
            catch(Exception ex)
            {
                Log.e(TAG, "[onActivityResult] exception");
                ex.printStackTrace();
            }
        }
        return false;
    }

    static public void prepareFromAsset(Context context, String key, String asset)
    {
        WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        DisplayMetrics metrics = new DisplayMetrics();
        wm.getDefaultDisplay().getMetrics(metrics);

        final int reqSize = Math.min(metrics.widthPixels, metrics.heightPixels);

        Bitmap sampleBitmap = ImageTools.decodeSampleAsset(context.getAssets(),
                asset, reqSize);
        if(sampleBitmap != null)
        {
            ImageWriter writer = new ImageWriter(context, key);
            writer.saveImages(sampleBitmap);
        }
        else
        {
            Log.e(TAG, "[prepareFromAsset] can't decode asset");
        }
    }

    static class ImageWriter
    {
        interface OnSaveImagesCompleteListener
        {
            void onComplete();
        }

        private Context context;
        private String key;

        private ProgressDialog progress;
        private OnSaveImagesCompleteListener listener;

        ImageWriter(Context context, String key)
        {
            this.context = context;
            this.key = key;
        }

        static public File imagePreviewFile(Context context, String key)
        {
            return new File(context.getExternalFilesDir(null), key + PREVIEW_SUFFIX);
        }

        static public File imagePortraitFile(Context context, String key)
        {
            return new File(context.getExternalFilesDir(null), key + PORTRAIT_SUFFIX);
        }

        static public File imageLandscapeFile(Context context, String key)
        {
            return new File(context.getExternalFilesDir(null), key + LANDSCAPE_SUFFIX);
        }

        class SaveImagesTask extends AsyncTask<String, Void, Void>
        {
            private boolean clean = false;

            public SaveImagesTask(boolean clean)
            {
                this.clean = clean;
            }

            @Override
            protected Void doInBackground(String... params)
            {
                Bitmap image = decodeSampleBitmap(params[0]);
                saveImages(image);
                if(clean)
                {
                    File file = new File(params[0]);
                    file.delete();
                }
                return null;
            }

            @Override
            protected void onPostExecute(Void param)
            {
                if(progress != null)
                    progress.dismiss();
                if(listener != null)
                    listener.onComplete();
            }
        }

        public void saveImagesAsync(String path, OnSaveImagesCompleteListener listener,
                                    boolean clean)
        {
            this.listener = listener;
            SaveImagesTask task = new SaveImagesTask(clean);
            task.execute(path);
            progress = ProgressDialog.show(context, "", context.getString(R.string.progress_preparing_images));
        }

        public void saveImages(Bitmap image)
        {
            if(image != null)
            {
                final String previewFile = savePreviewImage(image);
                final String portraitFile = savePortraitImage(image);
                final String landscapeFile = saveLandscapeImage(image);

                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
                SharedPreferences.Editor editor = prefs.edit();
                if(editor != null)
                {
                    editor.putString(key + PREVIEW_SUFFIX, previewFile);
                    editor.putString(key + PORTRAIT_SUFFIX, portraitFile);
                    editor.putString(key + LANDSCAPE_SUFFIX, landscapeFile);
                    editor.commit();
                }
            }
        }

        private Bitmap decodeSampleBitmap(String path)
        {
            WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
            DisplayMetrics metrics = new DisplayMetrics();
            wm.getDefaultDisplay().getMetrics(metrics);

            final int reqSize = Math.min(metrics.widthPixels, metrics.heightPixels);
            return ImageTools.decodeSampleBitmap(path, reqSize);
        }

        private String savePreviewImage(Bitmap image)
        {
            try
            {
                File file = imagePreviewFile(context, key);
                file.createNewFile();

                FileOutputStream stream = new FileOutputStream(file);
                image.compress(Bitmap.CompressFormat.JPEG, 100, stream);
                stream.close();

                return file.getPath();
            }
            catch(IOException ex)
            {
                Log.e(TAG, "[savePreviewImage] can't create image");
                ex.printStackTrace();
            }

            return "";
        }

        private String savePortraitImage(Bitmap image)
        {
            try
            {
                File file = imagePortraitFile(context, key);
                file.createNewFile();

                final float ratio = screenRatio();
                final int height = image.getHeight();
                final int width = Math.min((int)((float)height * ratio), image.getWidth());

                saveImage(image, width, height, file);

                return file.getPath();
            }
            catch(IOException ex)
            {
                Log.e(TAG, "[savePortraitImage] can't create image");
                ex.printStackTrace();
            }

            return "";
        }

        private String saveLandscapeImage(Bitmap image)
        {
            try
            {
                File file = imageLandscapeFile(context, key);
                file.createNewFile();

                final float ratio = screenRatio();
                final int width = image.getWidth();
                final int height = Math.min((int)((float)width * ratio), image.getHeight());

                saveImage(image, width, height, file);

                return file.getPath();
            }
            catch(IOException ex)
            {
                Log.e(TAG, "[saveLandscapeImage] can't create image");
                ex.printStackTrace();
            }

            return "";
        }

        private void saveImage(Bitmap source, int width, int height, File file)
                throws IOException
        {
            Matrix matrix = new Matrix();
            matrix.preScale(1.0f, -1.0f);
            Bitmap image = Bitmap.createBitmap(source, source.getWidth() / 2 - width / 2,
                    source.getHeight() / 2 - height / 2, width, height, matrix, true);

            FileOutputStream stream = new FileOutputStream(file);
            image.compress(Bitmap.CompressFormat.PNG, 100, stream);
            stream.close();
        }

        private float screenRatio()
        {
            WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
            DisplayMetrics metrics = new DisplayMetrics();
            wm.getDefaultDisplay().getMetrics(metrics);

            return (float)Math.min(metrics.heightPixels, metrics.widthPixels) /
                    (float) Math.max(metrics.heightPixels, metrics.widthPixels);
        }
    }

}