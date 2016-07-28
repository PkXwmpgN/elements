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

#include "pass_liquid.h"

#include <elements/rendering/state/state_macro.h>
#include <elements/rendering/utils/program_loader.h>
#include <elements/rendering/core/texture_policy.h>
#include <elements/rendering/core/texture_maker.h>

#include <elements/utils/std/enum.h>

#include <elements/assets/assets_storage.h>
#include <elements/assets/asset_texture.h>

namespace eps {
namespace experiment {
namespace liquid {

enum class program_enum : short
{
    // attributes
    a_vertex_xy = 0,
    a_vertex_uv = 1,
    // uniforms
    u_surface = 0,
    u_surface_background = 1,
    u_surface_color = 2,
    u_surface_texel = 3
};

bool pass_liquid::set_surface_background(const std::string & asset_name)
{
    auto asset = assets_storage::instance().read<asset_texture>(asset_name);
    if(asset)
    {
        using namespace rendering;

        auto maker = get_texture_maker<default_texture_policy>(asset->format());
        background_ = maker.construct(asset->pixels(), asset->size());

        return true;
    }
    return false;
}

void pass_liquid::set_surface_color(const math::vec4 & color)
{
    surface_color_ = color;
}

bool pass_liquid::initialize()
{
    return rendering::load_program("assets/shaders/experiments/liquid/liquid.prog", program_);
}

utils::unique<rendering::pass_target> pass_liquid::construct(const math::uvec2 & size)
{
    surface_texel_.x = 1.0f / size.x;
    surface_texel_.y = 1.0f / size.y;

    return nullptr;
}

void pass_liquid::process(float)
{
    EPS_STATE_SAMPLER_0(get_inputs().get_slot(rendering::pass_input_slot::input_0));
    EPS_STATE_SAMPLER_1(background_.get_product());
    EPS_STATE_PROGRAM(program_.get_product());

    program_.uniform_value(utils::to_int(program_enum::u_surface), 0);
    program_.uniform_value(utils::to_int(program_enum::u_surface_background), 1);
    program_.uniform_value(utils::to_int(program_enum::u_surface_color), surface_color_);
    program_.uniform_value(utils::to_int(program_enum::u_surface_texel), surface_texel_);

    square_.render(program_, utils::to_int(program_enum::a_vertex_xy),
                             utils::to_int(program_enum::a_vertex_uv));
}


} /* liquid */
} /* experiment */
} /* eps */
