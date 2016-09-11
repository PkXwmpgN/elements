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

#ifndef MATH_QUATERNION_H_INCLUDED
#define MATH_QUATERNION_H_INCLUDED

#include "geometric.h"
#include <glm/gtc/quaternion.hpp>

namespace eps {
namespace math {

using quat = glm::fquat;

inline quat rotate(const quat & q, const vec3 & axis, float degree)
{
    return glm::rotate(q, degree, axis);
}

inline mat4 to_mat4(const quat & q)
{
    return glm::mat4_cast(q);
}

inline quat look_rotation(const math::vec3 & direction, const math::vec3 & up)
{
    const math::vec3 z = normalize(direction);
    const math::vec3 y = normalize(up - z * dot(up, z));
    const math::vec3 x = cross(y, z);
    return quat(mat3(x, y, z));
}

} /* math */
} /* eps */

#endif // MATH_QUATERNION_H_INCLUDED
