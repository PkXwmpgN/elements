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

#include "pass_particles.h"

#include <elements/rendering/state/state_macro.h>
#include <elements/rendering/utils/program_loader.h>
#include <elements/utils/std/enum.h>
#include <elements/math/common.h>
#include <elements/assets/assets_storage.h>
#include <elements/assets/asset_texture.h>

#include <vector>

namespace eps {
namespace experiment {
namespace light {

enum class program_enum : short
{
    // attributes
    a_product_uv = 0,
    // uniforms
    u_positions = 0,
    u_velocities = 1,
    u_background = 2,
    u_texel = 3,
    u_size = 4
};

void pass_particles::set_count(size_t count)
{
    particles_count_ = count;
}

bool pass_particles::set_background(const std::string & asset_name)
{
    asset_texture asset = assets_storage::instance().read<asset_texture>(asset_name);
    if(asset.pixels())
    {
        texture_background_.set_data(asset.pixels(), asset.size(), asset.format());
        return true;
    }
    return false;
}

bool pass_particles::initialize()
{
    return rendering::load_program("shaders/experiments/light/particles.prog",
                                   program_);
}

utils::unique<rendering::pass_target> pass_particles::construct(const math::uvec2 & size)
{
    if(particles_count_)
    {
        const float scale = math::sqrt(float(size.x * size.y) / float(particles_count_));
        product_index_.construct(math::uvec2(size.x / scale, size.y / scale));

        particles_size_ = 2.0f * scale;
        texel_.x = 1.0f / size.x;
        texel_.y = 1.0f / size.y;
    }

    return nullptr;
}

void pass_particles::process(float)
{
    EPS_STATE_BLEND(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    EPS_STATE_SAMPLER_0(get_inputs().get_slot(rendering::pass_input_slot::input_0));
    EPS_STATE_SAMPLER_1(get_inputs().get_slot(rendering::pass_input_slot::input_1));
    EPS_STATE_SAMPLER_2(texture_background_.get_product());
    EPS_STATE_VERTICES(product_index_.get_product());
    EPS_STATE_PROGRAM(program_.get_product());

    program_.attribute_array_enable(utils::to_int(program_enum::a_product_uv));
    program_.attribute_array(utils::to_int(program_enum::a_product_uv), 0, 2, 0);
    program_.uniform_value(utils::to_int(program_enum::u_positions), 0);
    program_.uniform_value(utils::to_int(program_enum::u_velocities), 1);
    program_.uniform_value(utils::to_int(program_enum::u_background), 2);
    program_.uniform_value(utils::to_int(program_enum::u_texel), texel_);
    program_.uniform_value(utils::to_int(program_enum::u_size), particles_size_);

    glDrawArrays(GL_POINTS, 0, particles_count_);
}

} /* light */
} /* experiment */
} /* eps */
