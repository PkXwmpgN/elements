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

#include "logging.h"
#include "renderer_factory.h"
#include "liquid_renderer.h"

extern "C"
{
    // liquid
    JNIEXPORT jint JNICALL Java_com_yegorov_alexey_elements_api_ElementsLiquid_create(JNIEnv * env, jobject obj, jboolean preview);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLiquid_startup(JNIEnv * env, jobject obj, jint id, jint width, jint height, jint quality);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLiquid_background(JNIEnv * env, jobject obj, jint id, jstring asset);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLiquid_color(JNIEnv * env, jobject obj, jint id, jfloat r, jfloat g, jfloat b, jfloat a);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLiquid_touch(JNIEnv * env, jobject obj, jint id, jfloat x, jfloat y, jint action);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLiquid_acceleration(JNIEnv * env, jobject obj, jint id, jfloat x, jfloat y, jfloat z);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLiquid_destroy(JNIEnv * env, jobject obj, jint id);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLiquid_render(JNIEnv * env, jobject obj, jint id);
}
// ------------------------------------------------
// liquid

typedef renderer_factory<liquid_renderer> liquid_renderer_factory;
static liquid_renderer_factory liquid_renderer_factory_;

JNIEXPORT jint JNICALL Java_com_yegorov_alexey_elements_api_ElementsLiquid_create(JNIEnv *,
    jobject, jboolean preview)
{
    int id = liquid_renderer_factory_.open(preview);
    LOGI("[API][Liquid] create renderer (%d)", id);
    return id;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLiquid_startup(JNIEnv *,
    jobject, jint id, jint width, jint height, jint quality)
{
    auto renderer = liquid_renderer_factory_.get(id);
    if(renderer != nullptr)
        return renderer->startup(eps::math::uvec2(width, height), quality);
    LOGE("[API][Liquid] startup failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLiquid_background(JNIEnv * env,
    jobject, jint id, jstring asset)
{
    auto renderer = liquid_renderer_factory_.get(id);
    if(renderer != nullptr)
    {
        const char * asset_c = env->GetStringUTFChars(asset, nullptr);
        bool result = renderer->set_background(asset_c);
        env->ReleaseStringUTFChars(asset, asset_c);
        return result;
    }
    LOGE("[API][Liquid] set a background failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLiquid_color(JNIEnv *,
     jobject, jint id, jfloat r, jfloat g, jfloat b, jfloat a)
{
    auto renderer = liquid_renderer_factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->set_color(r, g, b, a);
        return true;
    }
    LOGE("[API][Liquid] set a color failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLiquid_touch(JNIEnv *,
    jobject, jint id, jfloat x, jfloat y, jint action)
{
    auto renderer = liquid_renderer_factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->touch(x, y, action);
        return true;
    }
    LOGE("[API][Liquid] touch failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLiquid_render(JNIEnv *,
    jobject, jint id)
{
    auto renderer = liquid_renderer_factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->render();
        return true;
    }
    LOGE("[API][Liquid] rendering failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLiquid_acceleration(JNIEnv *,
    jobject, jint id, jfloat x, jfloat y, jfloat z)
{
    auto renderer = liquid_renderer_factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->acceleration(x, y, z);
        return true;
    }

    LOGE("[API][Liquid] acceleration failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLiquid_destroy(JNIEnv *,
    jobject, jint id)
{
    if(!liquid_renderer_factory_.close(id))
    {
        LOGE("[API][Liquid] destroy failed: renderer is null (%d)", id);
        return false;
    }

    LOGI("[API][Liquid] destroy renderer (%d)", id);
    return true;
}
