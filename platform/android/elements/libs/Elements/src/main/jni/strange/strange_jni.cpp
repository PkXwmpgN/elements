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
#include "strange_renderer.h"

extern "C"
{
    // strange
    JNIEXPORT jint JNICALL Java_com_yegorov_alexey_elements_api_ElementsStrange_create(JNIEnv * env, jobject obj, jboolean preview);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsStrange_startup(JNIEnv * env, jobject obj, jint id, jint width, jint height, jint quantity);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsStrange_background(JNIEnv * env, jobject obj, jint id, jfloat r, jfloat g, jfloat b);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsStrange_gradient(JNIEnv * env, jobject obj, jint id, jfloat r1, jfloat g1, jfloat b1, jfloat r2, jfloat g2, jfloat b2);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsStrange_render(JNIEnv * env, jobject obj, jint id);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsStrange_destroy(JNIEnv * env, jobject obj, jint id);
}

// ------------------------------------------------------------
// strange

typedef renderer_factory<strange_renderer> strange_renderer_factory;
static strange_renderer_factory factory_;

JNIEXPORT jint JNICALL Java_com_yegorov_alexey_elements_api_ElementsStrange_create(JNIEnv *,
    jobject, jboolean preview)
{
    int id = factory_.open(preview);
    LOGI("[API][Strange] create renderer (%d)", id);
    return id;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsStrange_startup(JNIEnv *,
    jobject, jint id, jint width, jint height, jint quantity)
{
    auto renderer = factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->startup(eps::math::uvec2(width, height), quantity);
        return true;
    }

    LOGE("[API][Strange] startup failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsStrange_background(JNIEnv *,
    jobject, jint id, jfloat r, jfloat g, jfloat b)
{
    auto renderer = factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->set_background(eps::math::vec3(r, g, b));
        return true;
    }
    LOGE("[API][Strange] set a background failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsStrange_gradient(JNIEnv *,
    jobject, jint id, jfloat r1, jfloat g1, jfloat b1,
    jfloat r2, jfloat g2, jfloat b2)
{
    auto renderer = factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->set_gradient(eps::math::vec3(r1, g1, b1),
                               eps::math::vec3(r2, g2, b2));
        return true;
    }
    LOGE("[API][Strange] set a gradient failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsStrange_render(JNIEnv *,
    jobject, jint id)
{
    auto renderer = factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->render();
        return true;
    }
    LOGE("[API][Strange] rendering failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsStrange_destroy(JNIEnv *,
    jobject, jint id)
{
    if(!factory_.close(id))
    {
        LOGE("[API][Strange] destory failed: renderer is null (%d)", id);
        return false;
    }
    LOGI("[API][Strange] destory renderer (%d)", id);
    return true;
}
