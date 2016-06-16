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
#include "fire_renderer.h"

extern "C"
{
    // fire
    JNIEXPORT jint JNICALL Java_com_yegorov_alexey_elements_api_ElementsFire_create(JNIEnv * env, jobject obj, jboolean preview);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsFire_startup(JNIEnv * env, jobject obj, jint id, jint width, jint height, jint quality);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsFire_background(JNIEnv * env, jobject obj, jint id, jstring asset);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsFire_colorHot(JNIEnv * env, jobject obj, jint id, jfloat r, jfloat g, jfloat b);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsFire_colorCold(JNIEnv * env, jobject obj, jint id, jfloat r, jfloat g, jfloat b);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsFire_touch(JNIEnv * env, jobject obj, jint id, jfloat x, jfloat y, jint action);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsFire_render(JNIEnv * env, jobject obj, jint id);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsFire_destroy(JNIEnv * env, jobject obj, jint id);
}

// ------------------------------------------------------------
// fire
typedef renderer_factory<fire_renderer> fire_renderer_factory;
fire_renderer_factory fire_renderer_factory_;

JNIEXPORT jint JNICALL Java_com_yegorov_alexey_elements_api_ElementsFire_create(JNIEnv *,
    jobject, jboolean preview)
{
    int id = fire_renderer_factory_.open(preview);
    LOGI("[API][Fire] create renderer (%d)", id);
    return id;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsFire_startup(JNIEnv *,
    jobject, jint id, jint width, jint height, jint quality)
{
    auto renderer = fire_renderer_factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->startup(eps::math::uvec2(width, height), quality);
        return true;
    }

    LOGE("[API][Fire] startup failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsFire_background(JNIEnv * env,
    jobject, jint id, jstring asset)
{
    auto renderer = fire_renderer_factory_.get(id);
    if(renderer != nullptr)
    {
        const char * asset_c = env->GetStringUTFChars(asset, nullptr);
        bool result = renderer->set_background(asset_c);
        env->ReleaseStringUTFChars(asset, asset_c);
        return result;
    }
    LOGE("[API][Fire] set a background failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsFire_colorHot(JNIEnv *,
    jobject, jint id, jfloat r, jfloat g, jfloat b)
{
    auto renderer = fire_renderer_factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->set_color_hot(eps::math::vec3(r, g, b));
        return true;
    }
    LOGE("[API][Fire] set a hot color failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsFire_colorCold(JNIEnv *,
    jobject, jint id, jfloat r, jfloat g, jfloat b)
{
    auto renderer = fire_renderer_factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->set_color_cold(eps::math::vec3(r, g, b));
        return true;
    }
    LOGE("[API][Fire] set a cold color failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsFire_touch(JNIEnv *,
    jobject, jint id, jfloat x, jfloat y, jint action)
{
    auto renderer = fire_renderer_factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->touch(x, y, action);
        return true;
    }
    LOGE("[API][Fire] touch failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsFire_render(JNIEnv *,
    jobject, jint id)
{
    auto renderer = fire_renderer_factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->render();
        return true;
    }
    LOGE("[API][Fire] rendering failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsFire_destroy(JNIEnv *,
    jobject, jint id)
{
    if(!fire_renderer_factory_.close(id))
    {
        LOGE("[API][Fire] destroy failed: renderer is null (%d)", id);
        return false;
    }
    LOGI("[API][Fire] destroy renderer (%d)", id);
    return true;
}
