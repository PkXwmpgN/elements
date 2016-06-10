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

#ifndef UI_CONTROLS_BUTTON_H_INCLUDED
#define UI_CONTROLS_BUTTON_H_INCLUDED

#include <functional>

#include "ui/control.h"
#include "rendering/core/texture.h"
#include "rendering/core/texture_policy.h"
#include "rendering/core/program.h"
#include "rendering/primitives/square.h"

namespace eps {
namespace ui {

class button : public control
{
public:

    explicit button(control * parent = nullptr);

    void draw() override;
    bool touch(int x, int y, touch_action action) override;

    bool set_asset(const char * asset);
    void set_click(const std::function<void()> & handler);

private:

    enum class state
    {
        NONE = 0,
        PRESSED
    };

private:

    std::function<void()> click_;

    rendering::program program_face_;
    rendering::primitive::square square_;
    rendering::texture<rendering::default_texture_policy> texture_face_;

    state state_;
};

} /* ui */
} /* eps */

#endif // UI_BUTTON_H_INCLUDED
