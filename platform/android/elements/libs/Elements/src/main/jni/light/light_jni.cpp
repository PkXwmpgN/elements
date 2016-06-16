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
#include "light_renderer.h"

extern "C"
{
    // light
    JNIEXPORT jint JNICALL Java_com_yegorov_alexey_elements_api_ElementsLight_create(JNIEnv * env, jobject obj, jboolean preview);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLight_startup(JNIEnv * env, jobject obj, jint id, jint width, jint height, jint quantity);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLight_color(JNIEnv * env, jobject obj, jint id, jfloat r, jfloat g, jfloat b);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLight_background(JNIEnv * env, jobject obj, jint id, jstring asset);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLight_render(JNIEnv * env, jobject obj, jint id);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLight_touch(JNIEnv * env, jobject obj, jint id, jfloat x, jfloat y, jint action);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLight_destroy(JNIEnv * env, jobject obj, jint id);
}

// ------------------------------------------------------------
// light

typedef renderer_factory<light_renderer> light_renderer_factory;
static light_renderer_factory factory_;

JNIEXPORT jint JNICALL Java_com_yegorov_alexey_elements_api_ElementsLight_create(JNIEnv *,
    jobject, jboolean preview)
{
    int id = factory_.open(preview);
    LOGI("[API][Light] create renderer (%d)", id);
    return id;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLight_startup(JNIEnv *,
    jobject, jint id, jint width, jint height, jint count)
{
    auto renderer = factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->startup(eps::math::uvec2(width, height), count);
        return true;
    }

    LOGE("[API][Light] startup failed: renderer is null (%d)", id);
    return false;
}


JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLight_color(JNIEnv *,
    jobject, jint id, jfloat r, jfloat g, jfloat b)
{
    auto renderer = factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->set_color(eps::math::vec3(r, g, b));
        return true;
    }
    LOGE("[API][Light] set a color failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLight_background(JNIEnv * env,
    jobject, jint id, jstring asset)
{
    auto renderer = factory_.get(id);
    if(renderer != nullptr)
    {
        const char * asset_c = env->GetStringUTFChars(asset, nullptr);
        bool result = renderer->set_background(asset_c);
        env->ReleaseStringUTFChars(asset, asset_c);
        return result;
    }
    LOGE("[API][Light] set a background failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLight_render(JNIEnv *,
    jobject, jint id)
{
    auto renderer = factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->render();
        return true;
    }
    LOGE("[API][Light] rendering failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLight_touch(JNIEnv *,
    jobject, jint id, jfloat x, jfloat y, jint action)
{
    auto renderer = factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->touch(x, y, action);
        return true;
    }
    LOGE("[API][Light] touch failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsLight_destroy(JNIEnv *,
    jobject, jint id)
{
    if(!factory_.close(id))
    {
        LOGE("[API][Light] destroy failed: renderer is null (%d)", id);
        return false;
    }
    LOGI("[API][Light] destroy renderer (%d)", id);
    return true;
}
