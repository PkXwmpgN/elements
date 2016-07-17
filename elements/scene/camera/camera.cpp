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
#include "math/trigonometry.h"
#include "math/transform.h"

namespace eps {
namespace scene {

camera::camera()
    : up_(0.0f, 1.0f, 0.0)
    , fov_(math::radians(45.0f))
    , aspect_(4.0f / 3.0f)
    , near_(0.1f)
    , far_(100.0f)
{}

void camera::process()
{
    view_ = process_view();
    projection_ = process_projection();
}

math::mat4 camera::process_view()
{
    return math::look_at(get_eye(),
                         get_center(),
                         get_up());
}

math::mat4 camera::process_projection()
{
    return math::perspective(get_fov(),
                             get_aspect(),
                             get_plain_near(),
                             get_plain_far());
}

} /* scene */
} /* eps */
