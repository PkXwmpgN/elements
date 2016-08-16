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

#include "scene/graph/node.h"
#include "math/types.h"

namespace eps {
namespace scene {

class camera
{
public:

    explicit camera(const utils::link<node> & parent);
    camera(camera &&) = default;
    camera & operator=(camera &&) = default;
    virtual ~camera();

    void process(float dt);

public:

    void set_fov(float value) { fov_ = value; }
    void set_aspect(float value) { aspect_ = value; }
    void set_near(float value) { near_ = value; }
    void set_far(float value) { far_ = value; }

    float get_fov() const { return fov_; }
    float get_aspect() const { return aspect_; }
    float get_near() const { return near_; }
    float get_far() const { return far_; }

    const math::mat4 & get_projection() const { return projection_; }

    const utils::link<node> & get_node() const { return node_; }

private:

    virtual math::mat4 build_projection() = 0;

private:

    utils::link<node> node_;

    math::mat4 projection_;

    float fov_;
    float aspect_;
    float near_;
    float far_;
};

class camera_perspective : public camera
{
public:

    using camera::camera;

private:

    math::mat4 build_projection() final;
};

} /* scene */
} /* eps */

#endif // SCENE_CAMERA_H_INCLUDED
