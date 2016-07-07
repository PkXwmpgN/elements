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

#include "blur.h"
#include "rendering/passes/pass_target.h"
#include "rendering/state/state_macro.h"
#include "rendering/utils/program_loader.h"
#include "utils/std/enum.h"

namespace eps {
namespace rendering {
namespace effect {

enum class program_enum : short
{
    // attributes
    a_vertex_xy = 0,
    a_vertex_uv = 1,
    // uniforms
    u_source = 0,
    u_offset = 1
};

utils::unique<pass_target> blur_base::construct(const math::uvec2 & size)
{
    texel_.x = 1.0f / float(size.x);
    texel_.y = 1.0f / float(size.y);

    return nullptr;
}

bool blur_base::initialize()
{
    return load_program("assets/shaders/effects/blur.prog", program_);
}

void blur_base::process(float)
{
    EPS_STATE_SAMPLER_0(get_inputs().get_slot(pass_input_slot::input_0));
    EPS_STATE_PROGRAM(program_.get_product());

    program_.uniform_value(utils::to_int(program_enum::u_source), 0);
    program_.uniform_value(utils::to_int(program_enum::u_offset), offset(strong_, texel_));

    square_.render(program_, utils::to_int(program_enum::a_vertex_xy),
                             utils::to_int(program_enum::a_vertex_uv));
}

void blur_base::set_strong(float value)
{
    strong_ = value;
}

math::vec2 blur_v::offset(float strong, const math::vec2 & texel) const
{
    return math::vec2(0.0f, strong * texel.y);
}

math::vec2 blur_h::offset(float strong, const math::vec2 & texel) const
{
    return math::vec2(strong * texel.x, 0.0f);
}

void blur::set_strong(float value)
{
    if(auto link = link_blur_h_.lock())
        link->set_strong(value);
    if(auto link = link_blur_v_.lock())
        link->set_strong(value);
}

void blur::set_downsample(size_t value)
{
    downsample_ = value;
}

bool blur::initialize()
{
    passes_.initialize(2);

    link_blur_h_ = passes_.add_pass<blur_h>(get_inputs());
    if(link_blur_h_.expired())
        return false;

    link_blur_v_ = passes_.add_pass<blur_v>();
    if(link_blur_v_.expired())
        return false;

    passes_.add_dependency(link_blur_v_, link_blur_h_, pass_input_slot::input_0);

    return true;
}

utils::unique<pass_target> blur::construct(const math::uvec2 & size)
{
    if(downsample_)
    {
        const math::uvec2 down_size = size / downsample_;
        passes_.construct(down_size);
        return utils::make_unique<pass_target_simple>(down_size);
    }

    passes_.construct(size);
    return nullptr;
}

void blur::process(float dt)
{
    passes_.process(dt);
}

} /* effect */
} /* rendering */
} /* eps */
