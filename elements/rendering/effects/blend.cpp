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

#include "blend.h"
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
    u_source    = 0,
    u_transform = 1
};

bool blend::initialize()
{
    return load_program("assets/shaders/primitives/square_texture.prog", program_);
}

void blend::process(float)
{
    EPS_STATE_BLEND(sfactor_, dfactor_);
    EPS_STATE_SAMPLER_0(get_inputs().get_slot(pass_slot::slot_0));
    EPS_STATE_PROGRAM(program_.get_product());

    program_.uniform_value(utils::to_int(program_enum::u_source), 0);
    program_.uniform_value(utils::to_int(program_enum::u_transform), math::mat4(1.0f));

    square_.render(program_, utils::to_int(program_enum::a_vertex_xy),
                             utils::to_int(program_enum::a_vertex_uv));
}

void blend::set_factors(enum_type sfactor, enum_type dfactor)
{
    sfactor_ = sfactor;
    dfactor_ = dfactor;
}

} /* effect */
} /* rendering */
} /* eps */
