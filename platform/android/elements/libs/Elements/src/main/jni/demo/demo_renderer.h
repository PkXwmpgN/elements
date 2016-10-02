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

#ifndef PLATFORM_ANDROID_DEMO_RENDERER_H_INCLUDED
#define PLATFORM_ANDROID_DEMO_RENDERER_H_INCLUDED

#include <elements/timing/framerate.h>
#include <elements/utils/std/pointer.h>

#include <experiments/demo/renderer.h>

class demo_renderer
{
public:

    demo_renderer();

    bool startup(const eps::math::uvec2 & size);

    void render();
    void touch(float x, float y, int action, int finger);
    
    void set_rotation(float theta, float phi);

private:

    bool create_ui(const eps::math::uvec2 & size);

private:

    eps::timing::framerate rate_;
    eps::math::mat4 transform_touch_;
    eps::utils::unique<eps::experiment::demo::renderer> renderer_;
};

#endif // PLATFORM_ANDROID_DEMO_RENDERER_H_INCLUDED
