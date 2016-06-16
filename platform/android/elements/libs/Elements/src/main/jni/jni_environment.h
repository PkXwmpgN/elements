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

#ifndef PLATFORM_ANDROID_JNI_ENVIRONMENT_H_INCLUDED
#define PLATFORM_ANDROID_JNI_ENVIRONMENT_H_INCLUDED

#include <jni.h>

struct jni_environment
{
    explicit jni_environment(JavaVM * jvm)
        : jvm_(jvm)
        , jenv_(nullptr)
        , detach(false)
    {
        if(jvm_->GetEnv((void**)&jenv_, JNI_VERSION_1_4) != JNI_OK)
        {
            if(jvm_->AttachCurrentThread(&jenv_, nullptr) == JNI_OK)
                detach = true;
        }
    }

    ~jni_environment()
    {
        if(detach)
            jvm_->DetachCurrentThread();
    }

    JNIEnv * env() { return jenv_; }

private:

    JavaVM * jvm_;
    JNIEnv * jenv_;
    bool detach;
};

#endif // PLATFORM_ANDROID_JNI_ENVIRONMENT_H_INCLUDED
