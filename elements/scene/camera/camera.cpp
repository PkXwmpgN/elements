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

#include "camera.h"
#include "math/transform.h"
#include "math/trigonometry.h"

namespace eps {
namespace scene {

camera::camera(const utils::link<node> & parent)
    : node_(parent)
    , fov_(math::radians(60.0f))
    , aspect_(1.0f)
    , near_(0.1f)
    , far_(100.0f)
{}

camera::~camera()
{}

void camera::process(float)
{
    projection_ = build_projection();

    view_ = math::mat4(1.0f);
    auto sn = node_.lock();
    while(sn)
    {
        view_ = view_ * math::inverse(sn->get_local_matrix());
        sn = sn->get_parent().lock();
    }

    view_projection_ = projection_ * view_;
}

math::mat4 camera_perspective::build_projection()
{
    return glm::perspective(get_fov(), get_aspect(), get_near(), get_far());
}

} /* scene */
} /* eps */
