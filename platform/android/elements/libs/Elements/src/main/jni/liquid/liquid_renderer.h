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

#ifndef PLATFORM_ANDROID_LIQUID_RENDERER_H_INCLUDED
#define PLATFORM_ANDROID_LIQUID_RENDERER_H_INCLUDED

#include <elements/timing/framerate.h>
#include <elements/simulation/liquid/system.h>
#include <elements/ui/system.h>
#include <elements/ui/controls/button.h>
#include <elements/ui/controls/label.h>
#include <elements/utils/std/pointer.h>
#include <experiments/liquid/renderer.h>

class liquid_renderer
{
public:

    explicit liquid_renderer(bool preview);

    bool startup(const eps::math::uvec2 & size, size_t quality);

    void render();
    void acceleration(float x, float y, float z);
    void touch(float x, float y, int action);

    bool set_background(const char * asset);
    void set_color(float r, float g, float b, float a);

private:

    bool create_settings_ui(const eps::math::uvec2 & size);
    bool create_diagnostic_ui(const eps::math::uvec2 & size);

    void configure();

private:

    eps::timing::framerate rate_;
    eps::utils::pointer<eps::simulation::liquid::system> liquid_;

    eps::utils::unique<eps::experiment::liquid::renderer> renderer_;
    eps::utils::unique<eps::ui::system> ui_;
    eps::utils::link<eps::ui::label> label_fps_;

    eps::math::mat4 transform_;
    eps::math::mat4 transform_touch_;

    bool preview_;
};

#endif // PLATFORM_ANDROID_LIQUID_RENDERER_H_INCLUDED
