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

#include "positioning.h"
#include "math/transform.h"
#include "math/geometric.h"

namespace eps {
namespace scene {

void modifier_positioning::process(float)
{
    if(auto sn = get_node().lock())
    {
        sn->set_local_matrix(math::translate(position_) *
                             math::to_mat4(rotation_));
    }
}

void modifier_positioning::set_rotation(const math::vec3 & axis, float degree)
{
    rotation_ = math::rotate(rotation_, axis, degree);
}

void modifier_positioning::look_at(const math::vec3 & target,
                                   const math::vec3 & up)
{
    rotation_ = math::look_rotation(position_ - target, up);
}

void modifier_positioning::look_at(const math::vec3 & pos,
                                   const math::vec3 & target,
                                   const math::vec3 & up)
{
    set_position(pos);
    look_at(target, up);
}

} /* scene */
} /* eps */
