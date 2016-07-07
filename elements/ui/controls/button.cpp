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

#include "button.h"
#include "ui/system.h"
#include "rendering/state/state_macro.h"
#include "rendering/utils/program_loader.h"
#include "assets/assets_storage.h"
#include "assets/asset_texture.h"
#include "utils/std/enum.h"

namespace eps { namespace ui {

enum class square_enum : short
{
    // attributes
    a_vertex_xy = 0,
    a_vertex_uv = 1,
    // uniforms
    u_source = 0,
    u_transform = 1
};

button::button(control * parent)
    : control(parent)
    , square_(rendering::buffer_usage::STREAM_DRAW)
    , state_(state::NONE)
{
    rendering::load_program("assets/shaders/primitives/square_texture.prog",
                            program_face_);
}

bool button::set_asset(const char * asset)
{
    std::string asset_name(asset);
    auto face = assets_storage::instance().read<asset_texture>(asset_name);

    if(face)
    {
        auto & value = face.value();
        texture_face_.set_data(value.pixels(), value.size(), value.format());
        return true;
    }

    return false;
}

void button::set_click(const std::function<void()> & handler)
{
    click_ = handler;
}

void button::draw()
{
    EPS_STATE_BLEND(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    EPS_STATE_SAMPLER_0(texture_face_.get_product());
    EPS_STATE_PROGRAM(program_face_.get_product());

    program_face_.uniform_value(utils::to_int(square_enum::u_source), 0);
    program_face_.uniform_value(utils::to_int(square_enum::u_transform), get_system()->transform());

    math::vec2 pos = global_position();
    const float offset = state_ == state::PRESSED ? 0.5f : 0.0f;

    GLfloat vertices[] =
    {
        (pos.x),                (pos.y),                0.0f + offset, 1.0f,
        (pos.x + get_size().x), (pos.y),                0.5f + offset, 1.0f,
        (pos.x + get_size().x), (pos.y + get_size().y), 0.5f + offset, 0.0f,
        (pos.x),                (pos.y + get_size().y), 0.0f + offset, 0.0f
    };

    square_.construct(vertices);
    square_.render(program_face_, utils::to_int(square_enum::a_vertex_xy),
                                  utils::to_int(square_enum::a_vertex_uv));
}

bool button::touch(int x, int y, touch_action action)
{
    system * sys = get_system();

    math::vec2 pos = global_position();

    if((pos.x < x && x < pos.x + get_size().x &&
       (pos.y < y && y < pos.y + get_size().y)))
    {
        if(action == touch_action::DOWN)
        {
            sys->capture(this);
            state_ = state::PRESSED;
            return true;
        }
        else if(action == touch_action::UP)
        {
            if(click_)
                click_();
        }
    }

    if(sys->capture_test(this))
    {
        if(action == touch_action::UP)
        {
            sys->capture_release();
            state_ = state::NONE;
        }
        return true;
    }
    return false;
}

} /* ui */
} /* eps */
