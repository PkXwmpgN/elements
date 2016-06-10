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

#ifndef UI_CONTROLS_SLIDER_H_INCLUDED
#define UI_CONTROLS_SLIDER_H_INCLUDED

#include "ui/control.h"
#include "rendering/core/texture.h"
#include "rendering/core/program.h"
#include "rendering/primitives/square.h"

namespace eps { namespace ui {

class slider_model
{
public:

    virtual float get_value() const = 0;
    virtual void  set_value(float value) = 0;

    virtual ~slider_model() {}
};

class slider : public control
{
public:

    explicit slider(slider_model * model, control * parent = nullptr);

    void draw() override;
    bool touch(int x, int y, touch_action action) override;

    void set_color_slider(const math::vec4 & color);
    void set_color_tracker(const math::vec4 & color);

private:

    bool create_shaders();

    void draw_slider_begin(const math::vec2 & pos, float value);
    void draw_slider_end(const math::vec2 & pos, float value);
    void draw_tracker(const math::vec2 & pos, float value);

private:

    rendering::program program_slider_;
    rendering::program program_slider_tracker_;

    rendering::primitive::square square_;

    math::vec4 color_slider_;
    math::vec4 color_tracker_;

    std::unique_ptr<slider_model> model_;
};

} /* ui */
} /* eps */

#endif // UI_CONTROLS_SLIDER_H_INCLUDED
