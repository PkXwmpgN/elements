/*
The MIT License (MIT)
Copyright (c) 2016 Alexey Yegorov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
*/

#include <jni.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "asset_fs.h"
#include "preferences.h"
#include "metrics.h"

extern "C"
{
    // assets
    JNIEXPORT void JNICALL Java_com_yegorov_alexey_elements_api_Elements_initializeAssets(JNIEnv * env,
        jobject obj, jobject asset_manager);

    // preferences
    JNIEXPORT void JNICALL Java_com_yegorov_alexey_elements_api_Elements_initializePreferences(JNIEnv * env,
        jobject obj, jobject preferences);

    // metrics
    JNIEXPORT void JNICALL Java_com_yegorov_alexey_elements_api_Elements_initializeMetrics(JNIEnv * env,
        jobject obj, jobject metrics);
}

JNIEXPORT void JNICALL Java_com_yegorov_alexey_elements_api_Elements_initializeAssets(JNIEnv * env,
        jobject, jobject asset_manager)
{
    AAssetManager * manager = AAssetManager_fromJava(env, asset_manager);
    eps::assets_storage::instance().mount<asset_fs>("assets", manager);
}

JNIEXPORT void JNICALL Java_com_yegorov_alexey_elements_api_Elements_initializePreferences(JNIEnv * env,
    jobject, jobject preferences_obj)
{
    eps::preferences::init<preferences>(env, preferences_obj);
}

JNIEXPORT void JNICALL Java_com_yegorov_alexey_elements_api_Elements_initializeMetrics(JNIEnv * env,
    jobject, jobject metrics_obj)
{
    eps::metrics::init<metrics>(env, metrics_obj);
}
