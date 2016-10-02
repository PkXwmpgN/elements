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
#include "demo_renderer.h"

extern "C"
{
    // character
    JNIEXPORT jint JNICALL Java_com_yegorov_alexey_elements_api_ElementsDemo_create(JNIEnv * env, jobject obj);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsDemo_render(JNIEnv *, jobject, jint id);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsDemo_startup(JNIEnv * env, jobject obj, jint id, jint width, jint height);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsDemo_destroy(JNIEnv * env, jobject obj, jint id);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsDemo_rotation(JNIEnv * env, jobject obj, jint id, jfloat x, jfloat y, jfloat z);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsDemo_touch(JNIEnv * env, jobject obj, jint id, jfloat x, jfloat y, jint action, jint finger);
}


typedef renderer_factory<demo_renderer> demo_renderer_factory;
static demo_renderer_factory factory_;

JNIEXPORT jint JNICALL Java_com_yegorov_alexey_elements_api_ElementsDemo_create(JNIEnv *,
    jobject)
{
    int id = factory_.open();
    LOGI("[API][Demo] create (%d)", id);
    return id;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsDemo_startup(JNIEnv *,
    jobject, jint id, jint width, jint height)
{
    auto game = factory_.get(id);
    if(game != nullptr)
    {
        game->startup(eps::math::uvec2(width, height));
        return true;
    }

    LOGE("[API][Demo] startup failed: (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsDemo_render(JNIEnv *,
    jobject, jint id)
{
    auto game = factory_.get(id);
    if(game != nullptr)
    {
        game->render();
        return true;
    }
    LOGE("[API][Demo] rendering failed: (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsDemo_destroy(JNIEnv *,
    jobject, jint id)
{
    if(!factory_.close(id))
    {
        LOGE("[API][Demo] destory failed: (%d)", id);
        return false;
    }
    LOGI("[API][Demo] destory: (%d)", id);
    return true;
}


JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsDemo_touch(JNIEnv *,
    jobject, jint id, jfloat x, jfloat y, jint action, jint finger)
{
    auto game = factory_.get(id);
    if(game != nullptr)
    {
        game->touch(x, y, action, finger);
        return true;
    }
    LOGE("[API][Demo] touch failed: (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsDemo_rotation(JNIEnv *,
    jobject, jint id, jfloat, jfloat y, jfloat z)
{
    auto renderer = factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->set_rotation(y, z);
        return true;
    }
    LOGE("[API][Demo] rotation failed: renderer is null (%d)", id);
    return false;
}
