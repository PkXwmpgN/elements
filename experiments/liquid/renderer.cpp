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
#include "pass_particles.h"
#include "pass_metaballs.h"
#include "pass_liquid.h"
#include <elements/rendering/effects/blur.h>

#include <elements/math/common.h>
#include <elements/math/transform.h>

namespace eps {
namespace experiment {
namespace liquid {

bool renderer::initialize()
{
    using namespace rendering;

    // TODO: from xml + register user types

    passes_.initialize(6);

    link_particles_ = passes_.add_pass<pass_particles>();
    if(link_particles_.expired())
        return false;

    link_particles_blur_ = passes_.add_pass<effect::blur>();
    if(link_particles_blur_.expired())
        return false;

    auto link_metaballs = passes_.add_pass<pass_metaballs>();
    if(link_metaballs.expired())
        return false;

    link_surface_blur_0_ = passes_.add_pass<effect::blur>();
    if(link_surface_blur_0_.expired())
        return false;

    link_surface_blur_1_ = passes_.add_pass<effect::blur>();
    if(link_surface_blur_1_.expired())
        return false;

    link_liquid_ = passes_.add_pass<pass_liquid>();
    if(link_liquid_.expired())
        return false;

    passes_.add_dependency(link_particles_blur_, link_particles_, pass_input_slot::input_0);
    passes_.add_dependency(link_metaballs, link_particles_blur_, pass_input_slot::input_0);
    passes_.add_dependency(link_surface_blur_0_, link_metaballs, pass_input_slot::input_0);
    passes_.add_dependency(link_surface_blur_1_, link_surface_blur_0_, pass_input_slot::input_0);
    passes_.add_dependency(link_liquid_, link_surface_blur_1_, pass_input_slot::input_0);

    return true;
}

bool renderer::construct(const math::uvec2 & size,
                         const math::uvec2 & sim_size,
                         size_t texture_size)
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
        if(auto link = link_particles_.lock())
        {
            const math::mat4 transform = math::translate(-1.0f, -1.0f, 0.0f) *
                                         math::scale(2.0f, 2.0f, 1.0f) *
                                         math::scale(1.0f / sim_size.x, 1.0f / sim_size.y, 1.0f);

            link->set_particles_transform(transform);
            link->set_particles_size(2.5f * scale);
        }

        if(auto link = link_particles_blur_.lock())
            link->set_strong(0.75f * scale);
        if(auto link = link_surface_blur_0_.lock())
            link->set_strong(0.35f * scale);
        if(auto link = link_surface_blur_1_.lock())
            link->set_strong(0.25f * scale);

        passes_.construct(size_reduction);

        return true;
    }

    return false;
}

void renderer::render()
{
    passes_.process(0.0f);
}

void renderer::set_particles(sync::future<math::vec2> particles)
{
    if(auto link = link_particles_.lock())
        link->set_particles(std::move(particles));
}

bool renderer::set_surface_background(const std::string & asset_name)
{
    if(auto link = link_liquid_.lock())
        return link->set_surface_background(asset_name);
    return false;
}

void renderer::set_surface_color(const math::vec4 & color)
{
    if(auto link = link_liquid_.lock())
        link->set_surface_color(color);
}

} /* liquid */
} /* experiment */
} /* eps */
