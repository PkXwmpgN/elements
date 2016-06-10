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
#include "particles_product.h"
#include <elements/rendering/effects/gradient.h>
#include <elements/rendering/effects/blend.h>
#include <elements/rendering/effects/blur.h>
#include <elements/rendering/effects/tone.h>
#include <elements/rendering/effects/clear.h>
#include <elements/utils/std/enum.h>
#include <elements/math/common.h>

namespace eps {
namespace experiment {
namespace strange {

bool renderer::initialize(size_t particles_count)
{
    using namespace rendering;
    // TODO: from xml + register user types

    passes_.initialize(6);

    link_clear_ = passes_.add_pass<effect::clear>();
    if(link_clear_.expired())
        return false;

    auto link_particles = passes_.add_pass<particles_product>();
    if(link_particles.expired())
        return false;

    link_gradient_ = passes_.add_pass<effect::gradient>();
    if(link_gradient_.expired())
        return false;

    auto link_blur = passes_.add_pass<effect::blur>();
    if(link_blur.expired())
        return false;

    auto link_tone = passes_.add_pass<effect::tone>();
    if(link_tone.expired())
        return false;

    auto link_blend = passes_.add_pass<effect::blend>();
    if(link_blend.expired())
        return false;

    passes_.add_dependency(link_gradient_, link_particles, pass_input_slot::input_0);
    passes_.add_dependency(link_blur, link_gradient_, pass_input_slot::input_0);
    passes_.add_dependency(link_tone, link_blur, pass_input_slot::input_0);
    passes_.add_dependency(link_tone, link_gradient_, pass_input_slot::input_1);
    passes_.add_dependency(link_blend, link_tone, pass_input_slot::input_0);

    link_blur.lock()->set_strong(2.0f);
    link_blur.lock()->set_downsample(2);
    link_tone.lock()->set_tone(1.0f, 1.6f);
    link_particles.lock()->set_count(particles_count);

    return true;
}

bool renderer::construct(const math::uvec2 & size)
{
    passes_.construct(size);
    return true;
}

void renderer::render(float dt)
{
    passes_.process(dt);
}

void renderer::color_background(const math::vec3 & color)
{
    if(auto link = link_clear_.lock())
        link->set_color(math::vec4(color, 1.0f));
}

void renderer::color_gradient(const math::vec3 & color_1,
                              const math::vec3 & color_2)
{
    if(auto link = link_gradient_.lock())
        link->set_gradient(color_1, color_2);
}

} /* strange */
} /* experiment */
} /* eps */
