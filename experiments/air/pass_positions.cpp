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
#include <elements/rendering/core/texture_policy.h>
#include <elements/utils/std/enum.h>
#include <elements/math/common.h>

namespace eps {
namespace experiment {
namespace air {

enum class reset_enum : short
{
    // attributes
    a_vertex_xy = 0
};

enum class process_enum : short
{
    // attributes
    a_vertex_xy = 0,
    // uniforms
    u_positions = 0,
    u_velocities = 1,
    u_dt = 2,
};

void pass_positions::set_count(size_t particles_count)
{
    particles_count_ = particles_count;
}

bool pass_positions::initialize()
{
    return load_program("assets/shaders/experiments/air/positions_product_reset.prog",
                        program_reset_) &&
           load_program("assets/shaders/experiments/air/positions_product_process.prog",
                        program_process_);
}

utils::unique<rendering::pass_target> pass_positions::construct(const math::uvec2 & size)
{
    if(particles_count_)
    {
        reset_ = true;

        const float scale = math::sqrt(float(size.x * size.y) / float(particles_count_));

        using namespace rendering;
        return get_compute_target<default_texture_policy>(math::uvec2(size.x / scale,
                                                                      size.y / scale));
    }

    return nullptr;
}

void pass_positions::process(float dt)
{
    if(reset_)
    {
        pass_reset();
        reset_ = false;
    }
    else
    {
        pass_process(dt);
    }
}

void pass_positions::pass_reset()
{
    EPS_STATE_PROGRAM(program_reset_.get_product());
    square_.render(program_reset_, utils::to_int(reset_enum::a_vertex_xy));
}

void pass_positions::pass_process(float dt)
{
    EPS_STATE_SAMPLER_0(get_inputs().get_product(rendering::pass_slot::slot_0));
    EPS_STATE_SAMPLER_1(get_inputs().get_slot(rendering::pass_slot::slot_0));
    EPS_STATE_PROGRAM(program_process_.get_product());

    program_process_.uniform_value(utils::to_int(process_enum::u_positions), 0);
    program_process_.uniform_value(utils::to_int(process_enum::u_velocities), 1);
    program_process_.uniform_value(utils::to_int(process_enum::u_dt), dt);

    square_.render(program_process_, utils::to_int(process_enum::a_vertex_xy));
}

} /* air */
} /* experiments */
} /* eps */
