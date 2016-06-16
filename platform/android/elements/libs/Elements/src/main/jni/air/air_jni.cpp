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
#include "air_renderer.h"

extern "C"
{
    // air
    JNIEXPORT jint JNICALL Java_com_yegorov_alexey_elements_api_ElementsAir_create(JNIEnv * env, jobject obj, jboolean preview);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsAir_startup(JNIEnv * env, jobject obj, jint id, jint width, jint height, jint quantity);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsAir_colors(JNIEnv * env, jobject obj, jint id, jfloat r0, jfloat g0, jfloat b0, jfloat r1, jfloat g1, jfloat b1);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsAir_touch(JNIEnv * env, jobject obj, jint id, jfloat x, jfloat y, jint action);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsAir_render(JNIEnv * env, jobject obj, jint id);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsAir_destroy(JNIEnv * env, jobject obj, jint id);
}

// ------------------------------------------------------------
// air
typedef renderer_factory<air_renderer> air_renderer_factory;
static air_renderer_factory air_renderer_factory_;

JNIEXPORT jint JNICALL Java_com_yegorov_alexey_elements_api_ElementsAir_create(JNIEnv * /*env*/,
    jobject, jboolean preview)
{
    int id = air_renderer_factory_.open(preview);
    LOGI("[API][Air] create renderer (%d)", id);
    return id;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsAir_startup(JNIEnv *,
    jobject, jint id, jint width, jint height, jint quantity)
{
    auto renderer = air_renderer_factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->startup(eps::math::uvec2(width, height), quantity);
        return true;
    }

    LOGE("[API][Air] startup failed: renderer is null (%d)", id);
    return false;
}


JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsAir_colors(JNIEnv *,
    jobject, jint id, jfloat r0, jfloat g0, jfloat b0, jfloat r1, jfloat g1, jfloat b1)
{
    auto renderer = air_renderer_factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->set_colors(eps::math::vec3(r0, g0, b0), eps::math::vec3(r1, g1, b1));
        return true;
    }
    LOGE("[API][Air] set colors failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsAir_touch(JNIEnv *,
     jobject, jint id, jfloat x, jfloat y, jint action)
{
    auto renderer = air_renderer_factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->touch(x, y, action);
        return true;
    }
    LOGE("[API][Air] touch failed:renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsAir_render(JNIEnv *,
    jobject, jint id)
{
    auto renderer = air_renderer_factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->render();
        return true;
    }
    LOGE("[API][Air] rendering failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsAir_destroy(JNIEnv *,
    jobject, jint id)
{
    if(!air_renderer_factory_.close(id))
    {
        LOGE("[API][Air] destroy failed: renderer is null (%d)", id);
        return false;
    }
    LOGI("[API][Air] destroy renderer (%d)", id);
    return true;
}
