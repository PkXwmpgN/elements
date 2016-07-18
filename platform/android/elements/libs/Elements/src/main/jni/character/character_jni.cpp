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
#include "character_renderer.h"

extern "C"
{
    // character
    JNIEXPORT jint JNICALL Java_com_yegorov_alexey_elements_api_ElementsCharacter_create(JNIEnv * env, jobject obj, jboolean preview);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsCharacter_render(JNIEnv *, jobject, jint id);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsCharacter_startup(JNIEnv * env, jobject obj, jint id, jint width, jint height);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsCharacter_destroy(JNIEnv * env, jobject obj, jint id);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsCharacter_rotation(JNIEnv * env, jobject obj, jint id, jfloat x, jfloat y, jfloat z);
    JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsCharacter_model(JNIEnv * env, jobject obj, jint id, jstring asset);
}

// ------------------------------------------------------------
// character

typedef renderer_factory<character_renderer> character_renderer_factory;
static character_renderer_factory factory_;

JNIEXPORT jint JNICALL Java_com_yegorov_alexey_elements_api_ElementsCharacter_create(JNIEnv *,
    jobject, jboolean preview)
{
    int id = factory_.open(preview);
    LOGI("[API][Character] create renderer (%d)", id);
    return id;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsCharacter_startup(JNIEnv *,
    jobject, jint id, jint width, jint height)
{
    auto renderer = factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->startup(eps::math::uvec2(width, height));
        return true;
    }

    LOGE("[API][Character] startup failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsCharacter_render(JNIEnv *,
    jobject, jint id)
{
    auto renderer = factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->render();
        return true;
    }
    LOGE("[API][Character] rendering failed: renderer is null (%d)", id);
    return false;
}


JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsCharacter_rotation(JNIEnv *,
    jobject, jint id, jfloat, jfloat y, jfloat z)
{
    auto renderer = factory_.get(id);
    if(renderer != nullptr)
    {
        renderer->set_rotation(y, z);
        return true;
    }
    LOGE("[API][Character] rotation failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsCharacter_model(JNIEnv * env,
    jobject, jint id, jstring asset)
{
    auto renderer = factory_.get(id);
    if(renderer != nullptr)
    {
        const char * asset_c = env->GetStringUTFChars(asset, nullptr);
        bool result = renderer->set_model(std::string(asset_c));
        env->ReleaseStringUTFChars(asset, asset_c);
        return result;
    }
    LOGE("[API][Character] set a background failed: renderer is null (%d)", id);
    return false;
}

JNIEXPORT jboolean JNICALL Java_com_yegorov_alexey_elements_api_ElementsCharacter_destroy(JNIEnv *,
    jobject, jint id)
{
    if(!factory_.close(id))
    {
        LOGE("[API][Character] destory failed: renderer is null (%d)", id);
        return false;
    }
    LOGI("[API][Character] destory renderer (%d)", id);
    return true;
}
