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

    EPS_DESIGN_VISITABLE();

public:

    using entity::entity;

    void set_diffuse(const math::vec3 & diffuse) { diffuse_ = diffuse; }
    void set_specular(const math::vec3 & specular) { specular_ = specular; }
    void set_ambient(const math::vec3 & ambient) { ambient_ = ambient; }

    const math::vec3 & get_diffuse() const { return diffuse_; }
    const math::vec3 & get_specular() const { return specular_; }
    const math::vec3 & get_ambient() const { return ambient_; }

private:

    math::vec3 diffuse_  = math::vec3(1.0f, 1.0f, 1.0f);
    math::vec3 specular_ = math::vec3(1.0f, 1.0f, 1.0f);
    math::vec3 ambient_  = math::vec3(0.5f, 0.5f, 0.5f);
};

class light_point : public light
{
public:

    EPS_DESIGN_VISITABLE();

public:

    using light::light;
};

} /* scene */
} /* eps */

#endif // SCENE_LIGHT_H_INCLUDED
