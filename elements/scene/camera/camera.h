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

#ifndef SCENE_CAMERA_H_INCLUDED
#define SCENE_CAMERA_H_INCLUDED

#include "math/types.h"

namespace eps {
namespace scene {

class camera
{
public:

    void process();

    camera();
    camera(camera&&) = default;
    camera & operator=(camera&&) = default;
    virtual ~camera() {}

    void set_eye(const math::vec3 & value) { eye_ = value; }
    void set_center(const math::vec3 & value) { center_ = value; }
    void set_up(const math::vec3 & value) { up_ = value; }

    void set_fov(float fov) { fov_ = fov; }
    void set_aspect(float aspect) { aspect_ = aspect; }
    void set_plain_near(float near) { near_ = near; }
    void set_plain_far(float far) { far_ = far; }

    const math::vec3 & get_eye() const { return eye_; }
    const math::vec3 & get_center() const { return center_; }
    const math::vec3 & get_up() const { return up_; }

    float get_fov() const { return fov_; }
    float get_aspect() const { return aspect_; }
    float get_plain_near() const { return near_; }
    float get_plain_far() const { return far_; }

    const math::mat4 & get_view() const { return view_; }
    const math::mat4 & get_projection() const { return projection_; }

private:

    virtual math::mat4 process_view();
    virtual math::mat4 process_projection();

private:

    math::vec3 eye_;
    math::vec3 center_;
    math::vec3 up_;

    math::mat4 view_;
    math::mat4 projection_;

    float fov_;
    float aspect_;
    float near_;
    float far_;
};

} /* scene */
} /* eps */

#endif // SCENE_CAMERA_H_INCLUDED
