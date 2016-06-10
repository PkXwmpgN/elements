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

#include "renderer.h"
#include "pass_raymarching.h"
#include "pass_fire.h"

#include <elements/rendering/effects/blur.h>

namespace eps {
namespace experiment {
namespace fire {

bool renderer::initialize()
{
    using namespace rendering;

    passes_.initialize(3);

    link_raymarching_ = passes_.add_pass<pass_raymarching>();
    if(link_raymarching_.expired())
        return false;

    link_blur_ = passes_.add_pass<effect::blur>();
    if(link_blur_.expired())
        return false;

    link_fire_ = passes_.add_pass<pass_fire>();
    if(link_fire_.expired())
        return false;

    passes_.add_dependency(link_blur_, link_raymarching_, pass_input_slot::input_0);
    passes_.add_dependency(link_fire_, link_blur_, pass_input_slot::input_0);

    return true;
}

bool renderer::construct(const math::uvec2 & size, size_t texture_size)
{
    texture_size = std::max(texture_size, (size_t)256);

    math::uvec2 size_reduction;
    if(size.y < size.x)
    {
        size_reduction.x = texture_size;
        size_reduction.y = texture_size * ((float)size.y / (float)size.x);
    }
    else
    {
        size_reduction.y = texture_size;
        size_reduction.x = texture_size * ((float)size.x / (float)size.y);
    }

    if(size_reduction_ != size_reduction)
    {
        size_reduction_ = size_reduction;

        const float scale = texture_size / 128.0f;
        if(auto link = link_blur_.lock())
            link->set_strong(0.15f * scale);

        passes_.construct(size_reduction);

        return true;
    }

    return false;
}

void renderer::render(float dt)
{
    passes_.process(dt);
}

bool renderer::set_background(const char * background)
{
    if(auto link = link_fire_.lock())
        return link->set_background(background);
    return false;
}

void renderer::set_intensity(float intensity)
{
    if(auto link = link_raymarching_.lock())
        link->set_intensity(intensity);
}

void renderer::set_covering(float covering)
{
    if(auto link = link_raymarching_.lock())
        link->set_covering(covering);
}

void renderer::set_color_hot(const math::vec3 & color)
{
    if(auto link = link_raymarching_.lock())
        link->set_color_hot(color);
}

void renderer::set_color_cold(const math::vec3 & color)
{
    if(auto link = link_raymarching_.lock())
        link->set_color_cold(color);
}

} /* fire */
} /* experiment */
} /* eps */
