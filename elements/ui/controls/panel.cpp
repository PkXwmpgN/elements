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

#include "panel.h"
#include "ui/system.h"
#include "rendering/state/state_macro.h"
#include "rendering/core/opengl.h"
#include "rendering/utils/program_loader.h"
#include "utils/std/enum.h"

namespace eps { namespace ui {

enum program_enum : short
{
    // attributes
    a_vertex_xy = 0,
    a_vertex_uv = 1,
    // uniforms
    u_transform = 0,
    u_color = 1
};

panel::panel(control * parent)
    : control(parent)
    , square_(rendering::buffer_usage::STREAM_DRAW)
{
    rendering::load_program("assets/shaders/primitives/square_color.prog",
                            program_face_);
}

void panel::draw()
{
    EPS_STATE_BLEND(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    EPS_STATE_PROGRAM(program_face_.get_product());

    program_face_.uniform_value(utils::to_int(program_enum::u_transform), get_system()->transform());
    program_face_.uniform_value(utils::to_int(program_enum::u_color), color_);

    math::vec2 pos = global_position();

    GLfloat vertices[] =
    {
        (pos.x),                (pos.y),                0.0f, 1.0f,
        (pos.x + get_size().x), (pos.y),                1.0f, 1.0f,
        (pos.x + get_size().x), (pos.y + get_size().y), 1.0f, 0.0f,
        (pos.x),                (pos.y + get_size().y), 0.0f, 0.0f
    };
    square_.construct(vertices);
    square_.render(program_face_, utils::to_int(program_enum::a_vertex_xy),
                                  utils::to_int(program_enum::a_vertex_uv));
    control::draw();
}

void panel::set_color(const math::vec4 & color)
{
    color_ = color;
}

} /* ui */
} /* eps */
