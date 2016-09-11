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

#ifndef UI_CONTROL_H_INCLUDED
#define UI_CONTROL_H_INCLUDED

#include "math/types.h"
#include "types.h"
#include "utils/std/pointer.h"
#include <vector>

namespace eps {
namespace ui {

class system;

class control
{
public:

    virtual void draw();
    virtual bool touch(int x, int y, touch_action action, touch_finger finger);

    void set_position(float x, float y);
    void set_size(float w, float h);

    void set_visible(bool value);
    bool get_visible() const;

    const math::vec2 & get_position() const { return position_; }
    const math::vec2 & get_size() const { return size_; }

    template<typename _TControl, typename ..._TParams>
    utils::link<_TControl> add(_TParams&&... params)
    {
        auto control = utils::make_shared<_TControl>(std::forward<_TParams>(params)..., this);
        childs_.push_back(control);
        return control;
    }

protected:

    explicit control(control * parent = nullptr);
    virtual ~control();

    virtual const control * get_parent() const;
    virtual system * get_system();

    math::vec2 global_position() const;

private:

    std::vector<utils::pointer<control>> childs_;

    math::vec2 position_;
    math::vec2 size_;

    control * parent_;
    bool visible_;
};

} /* ui */
} /* eps */

#endif // UI_CONTROL_H_INCLUDED
