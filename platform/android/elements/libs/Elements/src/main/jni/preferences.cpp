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

#include "preferences.h"
#include "logging.h"
#include "jni_environment.h"

preferences::preferences(JNIEnv * env, jobject prefs)
    : jvm_(nullptr)
    , jpreferences_(env->NewGlobalRef(prefs))
    , jmethodID_getFloat_(nullptr)
    , jmethodID_edit_(nullptr)
    , jmethodID_edit_putFloat_(nullptr)
    , jmethodID_edit_apply_(nullptr)
{
    env->GetJavaVM(&jvm_);
    jclass prefs_class = env->GetObjectClass(jpreferences_);
    if(prefs_class != nullptr)
    {
        jmethodID_getFloat_ = env->GetMethodID(prefs_class,
            "getFloat", "(Ljava/lang/String;F)F");
        jmethodID_edit_ = env->GetMethodID(prefs_class,
            "edit", "()Landroid/content/SharedPreferences$Editor;");
    }

    jclass edit_class = env->FindClass("android/content/SharedPreferences$Editor");
    if(edit_class != nullptr)
    {
        jmethodID_edit_putFloat_ = env->GetMethodID(edit_class,
            "putFloat", "(Ljava/lang/String;F)Landroid/content/SharedPreferences$Editor;");
        jmethodID_edit_apply_ = env->GetMethodID(edit_class,
            "apply", "()V");
    }
}

preferences::~preferences()
{
    jni_environment guard(jvm_);
    JNIEnv * env = guard.env();
    if(env)
        env->DeleteGlobalRef(jpreferences_);
}

float preferences::get_float(const std::string & key, float default_value) const
{
    if(jvm_ == nullptr)
    {
        LOGE("[preferences::get_float] JVM is null");
        return default_value;
    }

    jni_environment guard(jvm_);
    JNIEnv * env = guard.env();

    if(env == nullptr)
    {
        LOGE("[preferences::get_float] JNIEnv is null");
        return default_value;
    }

    if(jmethodID_getFloat_ == nullptr)
    {
        LOGE("[preferences::get_float] jmethodID_getFloat_ is null");
        return default_value;
    }

    const jstring jkey = env->NewStringUTF(key.c_str());
    if(jkey == nullptr)
    {
        LOGE("[preferences::get_float] key is null");
        return default_value;
    }

    return env->CallFloatMethod(jpreferences_, jmethodID_getFloat_,
                                jkey, default_value);
}

void preferences::set_float(const std::string & key, float value)
{
    if(jvm_ == nullptr)
    {
        LOGE("[preferences::set_float] JVM is null");
        return;
    }

    jni_environment guard(jvm_);
    JNIEnv * env = guard.env();
    if(env == nullptr)
    {
        LOGE("[preferences::set_float] JNIEnv is null");
        return;
    }

    jobject editor = env->CallObjectMethod(jpreferences_, jmethodID_edit_);
    if(editor == nullptr)
    {
        LOGE("[preferences::set_float] Editor is null");
        return;
    }

    if(jmethodID_edit_putFloat_ == nullptr)
    {
        LOGE("[preferences::set_float] jmethodID_edit_putFloat_ is null");
        return;
    }

    if(jmethodID_edit_apply_ == nullptr)
    {
        LOGE("[preferences::set_float] jmethodID_edit_apply_ is null");
        return;
    }

    const jstring jkey = env->NewStringUTF(key.c_str());
    if(jkey == nullptr)
    {
        LOGE("[preferences::set_float] key is null");
        return;
    }

    env->CallObjectMethod(editor, jmethodID_edit_putFloat_, jkey, value);
    env->CallVoidMethod(editor, jmethodID_edit_apply_);
}
