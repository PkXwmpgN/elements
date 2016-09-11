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

#include "control.h"
#include "system.h"

namespace eps {
namespace ui {

control::control(control * parent)
    : parent_(parent)
    , visible_(true)
{}

control::~control()
{}

const control * control::get_parent() const
{
    return parent_;
}

system * control::get_system()
{
    return parent_ ? parent_->get_system() : nullptr;
}

void control::draw()
{
    for(auto & child : childs_)
        if(child->get_visible())
            child->draw();
}

bool control::touch(int x, int y, touch_action action, touch_finger finger)
{
    for(auto & child : childs_)
        if(child->get_visible() && child->touch(x, y, action, finger))
            return true;
    return false;
}

void control::set_position(float x, float y)
{
    position_.x = x;
    position_.y = y;
}

void control::set_size(float w, float h)
{
    size_.x = w;
    size_.y = h;
}

void control::set_visible(bool value)
{
    visible_ = value;
}

bool control::get_visible() const
{
    return visible_;
}

math::vec2 control::global_position() const
{
    math::vec2 result = position_;

    const control * parent = parent_;
    while(parent)
    {
        result += parent->get_position();
        parent = parent->get_parent();
    }

    return result;
}

} /* ui */
} /* eps */
