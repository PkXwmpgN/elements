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

#include "slider.h"
#include "ui/system.h"
#include "rendering/state/state_macro.h"
#include "rendering/utils/program_loader.h"
#include "utils/std/enum.h"
#include "math/common.h"

namespace eps {
namespace ui {

enum class program_enum : short
{
    // attributes
    a_vertex_xy = 0,
    a_vertex_uv = 1,
    // uniforms
    u_transform = 0,
    u_color = 1
};

slider::slider(slider_model * model, control * parent)
    : control(parent)
    , square_(rendering::buffer_usage::STREAM_DRAW)
    , color_slider_(0.8f, 0.8f, 0.8f, 1.0f)
    , color_tracker_(0.8f, 0.8f, 0.8f, 1.0f)
    , model_(model)
{
    rendering::load_program("shaders/primitives/square_color.prog",
                            program_slider_);
    rendering::load_program("shaders/primitives/circle_color.prog",
                            program_slider_tracker_);
}

void slider::set_color_slider(const math::vec4 & color)
{
    color_slider_ = color;
}

void slider::set_color_tracker(const math::vec4 & color)
{
    color_tracker_ = color;
}

void slider::draw()
{
    EPS_STATE_BLEND(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    math::vec2 pos = global_position();
    const float value = model_->get_value();

    draw_slider_end(pos, value);
    draw_slider_begin(pos, value);
    draw_tracker(pos, value);
}

void slider::draw_slider_end(const math::vec2 & pos, float value)
{
    EPS_STATE_PROGRAM(program_slider_.get_product());

    program_slider_.uniform_value(utils::to_int(program_enum::u_transform), get_system()->transform());
    program_slider_.uniform_value(utils::to_int(program_enum::u_color), color_slider_);

    float offset_y = get_size().y * 0.45f;
    float offset_x = get_size().x * value;

    GLfloat vertices[] =
    {
        (pos.x + offset_x), (pos.y + offset_y),                    0.0f, 1.0f,
        (pos.x + get_size().x), (pos.y + offset_y),                1.0f, 1.0f,
        (pos.x + get_size().x), (pos.y + get_size().y - offset_y), 1.0f, 0.0f,
        (pos.x + offset_x), (pos.y + get_size().y - offset_y),     0.0f, 0.0f
    };
    square_.construct(vertices);
    square_.render(program_slider_, utils::to_int(program_enum::a_vertex_xy),
                                    utils::to_int(program_enum::a_vertex_uv));
}


void slider::draw_slider_begin(const math::vec2 & pos, float value)
{
    EPS_STATE_PROGRAM(program_slider_.get_product());

    program_slider_.uniform_value(utils::to_int(program_enum::u_transform), get_system()->transform());
    program_slider_.uniform_value(utils::to_int(program_enum::u_color), color_tracker_);

    float offset_y = get_size().y * 0.4f;
    float offset_x = get_size().x * value;

    GLfloat vertices[] =
    {
        (pos.x),            (pos.y + offset_y),                0.0f, 1.0f,
        (pos.x + offset_x), (pos.y + offset_y),                1.0f, 1.0f,
        (pos.x + offset_x), (pos.y + get_size().y - offset_y), 1.0f, 0.0f,
        (pos.x),            (pos.y + get_size().y - offset_y), 0.0f, 0.0f
    };
    square_.construct(vertices);
    square_.render(program_slider_, utils::to_int(program_enum::a_vertex_xy),
                                    utils::to_int(program_enum::a_vertex_uv));
}

void slider::draw_tracker(const math::vec2 & pos, float value)
{
    EPS_STATE_PROGRAM(program_slider_tracker_.get_product());

    program_slider_tracker_.uniform_value(utils::to_int(program_enum::u_transform), get_system()->transform());
    program_slider_tracker_.uniform_value(utils::to_int(program_enum::u_color), color_tracker_);

    float offset = get_size().y * 0.8f;
    math::vec2 tracker(pos.x + (get_size().x - offset) * value, pos.y + (get_size().y - offset) * 0.5f);
    GLfloat vertices[] =
    {
        (tracker.x),          (tracker.y),          0.0f, 1.0f,
        (tracker.x + offset), (tracker.y),          1.0f, 1.0f,
        (tracker.x + offset), (tracker.y + offset), 1.0f, 0.0f,
        (tracker.x),          (tracker.y + offset), 0.0f, 0.0f
    };
    square_.construct(vertices);
    square_.render(program_slider_tracker_, utils::to_int(program_enum::a_vertex_xy),
                                            utils::to_int(program_enum::a_vertex_uv));
}

bool slider::touch(int x, int y, touch_action action)
{
    system * sys = get_system();

    math::vec2 pos = global_position();

    if((pos.x < x && x < pos.x + get_size().x &&
       (pos.y < y && y < pos.y + get_size().y)))
    {
        if(action == touch_action::DOWN)
        {
            sys->capture(this);
            return true;
        }
    }

    if(sys->capture_test(this))
    {
        if(action == touch_action::UP)
        {
            sys->capture_release();
        }
        else if(action == touch_action::MOVE)
        {
            const float value = (math::clamp(x, pos.x, pos.x + get_size().x) - pos.x) / get_size().x;
            model_->set_value(value);
        }
        return true;
    }
    return false;
}

} /* ui */
} /* eps */
