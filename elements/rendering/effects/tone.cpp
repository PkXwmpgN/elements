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

#include "tone.h"
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
    u_source_0 = 0,
    u_source_1 = 1,
    u_exposure = 2,
    u_gamma    = 3
};

void tone::set_tone(float exposure, float gamma)
{
    exposure_ = exposure;
    gamma_ = gamma;
}

bool tone::initialize()
{
    return load_program("assets/shaders/effects/tone.prog", program_);
}

void tone::process(float)
{
    EPS_STATE_SAMPLER_0(get_inputs().get_slot(pass_slot::slot_0));
    EPS_STATE_SAMPLER_1(get_inputs().get_slot(pass_slot::slot_1));
    EPS_STATE_PROGRAM(program_.get_product());

    program_.uniform_value(utils::to_int(program_enum::u_source_0), 0);
    program_.uniform_value(utils::to_int(program_enum::u_source_1), 1);
    program_.uniform_value(utils::to_int(program_enum::u_exposure), exposure_);
    program_.uniform_value(utils::to_int(program_enum::u_gamma), gamma_);

    square_.render(program_, utils::to_int(program_enum::a_vertex_xy),
                             utils::to_int(program_enum::a_vertex_uv));
}

} /* effect */
} /* rendering */
} /* eps */
