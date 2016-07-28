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

#include "label.h"

#include "ui/system.h"
#include "ui/freetype/manager.h"
#include "ui/freetype/line.h"

#include "rendering/state/state_macro.h"
#include "rendering/core/opengl.h"
#include "rendering/utils/program_loader.h"
#include "rendering/core/texture_maker.h"
#include "rendering/core/texture_policy.h"

#include "utils/std/enum.h"

namespace eps {
namespace ui {

enum class program_enum : short
{
    // attributes
    a_veretx_xy = 0,
    a_vertex_uv = 1,
    // uniforms
    u_source = 0,
    u_transform = 1,
    u_color = 2
};

label::label(control * parent)
    : control(parent)
    , square_(rendering::buffer_usage::STREAM_DRAW)
    , font_(freetype::INVALID_FACE_HANDLE)
    , color_(1.0f, 0.0f, 0.0f, 1.0f)
    , height_(12)
    , valignment_(valignment::TOP)
    , halignment_(halignment::LEFT)
{
    rendering::load_program("assets/shaders/primitives/square_texture_alpha.prog",
                            program_face_);
}

bool label::set_font(const std::string & ttf_asset, int height)
{
   height_ = height;
   font_ = freetype::manager::instance().get_face(ttf_asset);
   return font_ != freetype::INVALID_FACE_HANDLE;
}

void label::set_color(const math::vec4 & color)
{
    color_ = color;
}

void label::set_alignment(halignment haling, valignment valign)
{
    // TODO: alignments not implemented
    halignment_ = haling;
    valignment_ = valign;
}

bool label::set_text(const std::string & text)
{
    if(font_ != freetype::INVALID_FACE_HANDLE)
    {
        freetype::line line = freetype::manager::instance().get_line(text, font_, height_);

        using namespace rendering;

        auto maker = get_texture_maker<alpha_texture_policy>();
        texture_face_ = maker.construct(line.get_data(), line.get_size());

        return true;
    }
    return false;
}

void label::draw()
{
    EPS_STATE_BLEND(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    EPS_STATE_SAMPLER_0(texture_face_.get_product());
    EPS_STATE_PROGRAM(program_face_.get_product());

    program_face_.uniform_value(utils::to_int(program_enum::u_source), 0);
    program_face_.uniform_value(utils::to_int(program_enum::u_transform), get_system()->transform());
    program_face_.uniform_value(utils::to_int(program_enum::u_color), color_);

    math::vec2 pos = global_position();

    const math::uvec2 & s = texture_face_.get_size();

    GLfloat vertices[] =
    {
        (pos.x),       (pos.y),       0.0f, 1.0f,
        (pos.x + s.x), (pos.y),       1.0f, 1.0f,
        (pos.x + s.x), (pos.y + s.y), 1.0f, 0.0f,
        (pos.x),       (pos.y + s.y), 0.0f, 0.0f
    };

    square_.construct(vertices);
    square_.render(program_face_, utils::to_int(program_enum::a_veretx_xy),
                                  utils::to_int(program_enum::a_vertex_uv));
}

} /* ui */
} /* eps */
