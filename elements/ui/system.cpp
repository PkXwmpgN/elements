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

#include "system.h"
#include "math/transform.h"

namespace eps {
namespace ui {

system::system()
    : control(nullptr)
{}

bool system::touch(int x, int y, touch_action action, touch_finger finger)
{
    if(capture_[utils::to_int(finger)])
        return capture_[utils::to_int(finger)]->touch(x, y, action, finger);
    return control::touch(x, y, action, finger);
}

bool system::construct(const math::uvec2 & size)
{
    transform_ = math::translate(-1.0f, -1.0f, 0.0f) *
                 math::scale(2.0f, 2.0f, 1.0f) *
                 math::scale(1.0f / size.x, 1.0f / size.y, 1.0f);
    return true;
}

system * system::get_system()
{
    return this;
}

void system::capture(control * object, touch_finger finger)
{
    capture_[utils::to_int(finger)] = object;
}

void system::capture_release(touch_finger finger)
{
    capture_[utils::to_int(finger)] = nullptr;
}

bool system::capture_test(control * object, touch_finger finger) const
{
    return capture_[utils::to_int(finger)] == object;
}

} /* ui */
} /* eps */
