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

#include "pass_fire.h"

#include <elements/rendering/state/state_macro.h>
#include <elements/rendering/utils/program_loader.h>

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
    u_fire = 0,
    u_background = 1,
    u_texel = 2
};

bool pass_fire::set_background(const char * background)
{
    std::string asset_name(background);
    auto asset = assets_storage::instance().read<asset_texture>(asset_name);

    if(asset)
    {
        auto & value = asset.value();
        texture_background_.set_data(value.pixels(), value.size(), value.format());
        return true;
    }
    return false;
}

bool pass_fire::initialize()
{
    return rendering::load_program("assets/shaders/experiments/fire/fire.prog", program_);
}

void pass_fire::process(float)
{
    EPS_STATE_SAMPLER_0(get_inputs().get_slot(rendering::pass_input_slot::input_0));
    EPS_STATE_SAMPLER_1(texture_background_.get_product());
    EPS_STATE_PROGRAM(program_.get_product());

    program_.uniform_value(utils::to_int(program_enum::u_fire), 0);
    program_.uniform_value(utils::to_int(program_enum::u_background), 1);
    program_.uniform_value(utils::to_int(program_enum::u_texel), texel_);

    square_.render(program_, utils::to_int(program_enum::a_vertex_xy),
                             utils::to_int(program_enum::a_vertex_uv));
}

utils::unique<rendering::pass_target> pass_fire::construct(const math::uvec2 & size)
{
    texel_.x = 1.0f / size.x;
    texel_.y = 1.0f / size.y;

    return nullptr;
}

} /* fire */
} /* experiment */
} /* eps */
