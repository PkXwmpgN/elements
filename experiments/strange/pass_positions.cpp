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

#include "pass_positions.h"

#include <elements/rendering/computation/compute_target.h>
#include <elements/rendering/state/state_macro.h>
#include <elements/rendering/utils/program_loader.h>
#include <elements/utils/std/enum.h>

namespace eps {
namespace experiment {
namespace strange {

enum class program_enum : short
{
    // attributes
    a_vertex_xy = 0,
    // uniforms
    u_attractor_a = 0,
    u_attractor_b = 1,
    u_attractor_c = 2,
    u_attractor_d = 3
};

bool pass_positions::initialize()
{
    return rendering::load_program("assets/shaders/experiments/strange/positions_product_process.prog",
                                   program_process_);
}

utils::unique<rendering::pass_target> pass_positions::construct(const math::uvec2 & size)
{
    using namespace rendering;
    return get_compute_target<default_texture_policy>(size);
}

void pass_positions::process(float dt)
{
    EPS_STATE_PROGRAM(program_process_.get_product());

    attractors_.process(dt);

    program_process_.uniform_value(utils::to_int(program_enum::u_attractor_a), attractors_.a());
    program_process_.uniform_value(utils::to_int(program_enum::u_attractor_b), attractors_.b());
    program_process_.uniform_value(utils::to_int(program_enum::u_attractor_c), attractors_.c());
    program_process_.uniform_value(utils::to_int(program_enum::u_attractor_d), attractors_.d());

    square_.render(program_process_, utils::to_int(program_enum::a_vertex_xy));
}

} /* strange */
} /* experiment */
} /* eps */
