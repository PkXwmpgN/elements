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

#ifndef SCENE_LIGHT_H_INCLUDED
#define SCENE_LIGHT_H_INCLUDED

#include "math/types.h"
#include "scene/entity/entity.h"

namespace eps {
namespace scene {

class light : public entity
{
public:

    SNAPE_VISITABLE();

public:

    using entity::entity;

    void set_intensity(const math::vec3 & intensity) { intensity_ = intensity; }
    void set_enabled(bool enable) { enabled_ = enable; }

    const math::vec3 & get_intensity() const { return intensity_; }
    bool get_enabled() const { return enabled_; }

private:

    math::vec3 intensity_ = math::vec3(1.0f, 1.0f, 1.0f);
    bool enabled_ = true;
};

class light_point : public light
{
public:

    SNAPE_VISITABLE();

public:

    using light::light;

    void  set_range(float range) { range_ = range; }
    float get_range() const { return range_; }

private:

    float range_ = 1.0f;
};

} /* scene */
} /* eps */

#endif // SCENE_LIGHT_H_INCLUDED
