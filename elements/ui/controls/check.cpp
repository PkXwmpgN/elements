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

#include "check.h"
#include "ui/system.h"

namespace eps { namespace ui {

bool check::touch(int x, int y, touch_action action, touch_finger finger)
{
    system * sys = get_system();

    math::vec2 pos = global_position();

    if((pos.x < x && x < pos.x + get_size().x &&
       (pos.y < y && y < pos.y + get_size().y)))
    {
        if(action == touch_action::DOWN)
        {
            sys->capture(this, finger);
            return true;
        }
        else if(action == touch_action::UP)
        {
            if(state_ == state::PRESSED)
                state_ = state::NONE;
            else if(state_ == state::NONE)
                state_ = state::PRESSED;

            if(click_)
                click_(state_);
        }
    }

    if(sys->capture_test(this, finger))
    {
        if(action == touch_action::UP)
            sys->capture_release(finger);
        return true;
    }
    return false;
}

} /* ui */
} /* eps */
