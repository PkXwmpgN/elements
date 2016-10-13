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

#include "pass_raymarching.h"

#include <elements/rendering/state/state_macro.h>
#include <elements/rendering/utils/program_loader.h>
#include <elements/rendering/core/texture_policy.h>
#include <elements/rendering/core/texture_maker.h>
#include <elements/utils/std/enum.h>
#include <elements/assets/assets_storage.h>
#include <elements/assets/asset_texture.h>

namespace eps {
namespace experiment {
namespace fire {

enum class program_enum : short
{
    // atttributes
    a_vertex_xy = 0,
    a_vertex_uv = 1,
    // uniforms
    u_noise = 0,
    u_dt = 1,
    u_intensity = 2,
    u_covering = 3,
    u_texel = 4,
    u_color_hot = 5,
    u_color_cold = 6
};

void pass_raymarching::set_intensity(float intensity)
{
    fire_intensity_ = intensity;
}

void pass_raymarching::set_covering(float covering)
{
    fire_covering_ = covering;
}

void pass_raymarching::set_color_hot(const math::vec3 & color)
{
    color_hot_ = color;
}

void pass_raymarching::set_color_cold(const math::vec3 & color)
{
    color_cold_ = color;
}

bool pass_raymarching::initialize()
{
    const std::string asset_name("assets/textures/noise.png");
    auto asset = assets_storage::instance().read<asset_texture>(asset_name);

    if(!asset)
        return false;

    using namespace rendering;

    auto maker = get_texture_maker<repeat_texture_policy>();
    noise_ = maker.construct(asset->pixels(), asset->size());

    return load_program("assets/shaders/experiments/fire/raymarching.prog", program_);
}

void pass_raymarching::process(float dt)
{
    EPS_STATE_SAMPLER_0(noise_.get_product());
    EPS_STATE_PROGRAM(program_.get_product());

    program_.uniform_value(utils::to_int(program_enum::u_noise), 0);
    program_.uniform_value(utils::to_int(program_enum::u_texel), texel_);
    program_.uniform_value(utils::to_int(program_enum::u_dt), dt);
    program_.uniform_value(utils::to_int(program_enum::u_intensity), fire_intensity_);
    program_.uniform_value(utils::to_int(program_enum::u_covering), fire_covering_);
    program_.uniform_value(utils::to_int(program_enum::u_color_hot), color_hot_);
    program_.uniform_value(utils::to_int(program_enum::u_color_cold), color_cold_);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    square_.render(program_, utils::to_int(program_enum::a_vertex_xy),
                             utils::to_int(program_enum::a_vertex_uv));
}

utils::unique<rendering::pass_target> pass_raymarching::construct(const math::uvec2 & size)
{
    texel_.x = 1.0f / size.x;
    texel_.y = 1.0f / size.y;

    return nullptr;
}

} /* fire */
} /* experiment */
} /* eps */
