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

#ifndef MATH_TRANSFORM_H_INCLUDED
#define MATH_TRANSFORM_H_INCLUDED

#include "types.h"
#include <glm/gtx/transform.hpp>

namespace eps {
namespace math {

inline mat4 rotate(float angle, float x, float y, float z)
{
    return glm::rotate(angle, vec3(x, y, z));
}

inline mat4 rotate(float angle, const vec3 & value)
{
    return glm::rotate(angle, value);
}

inline mat4 scale(float x, float y, float z)
{
    return glm::scale(vec3(x, y, z));
}

inline mat4 scale(const vec3 & value)
{
    return glm::scale(value);
}

inline mat4 translate(float x, float y, float z)
{
    return glm::translate(vec3(x, y, z));
}

inline mat4 translate(const vec3 & value)
{
    return glm::translate(value);
}

inline mat4 perspective(float fovy, float aspect, float near, float far)
{
    return glm::perspective(fovy, aspect, near, far);
}

inline mat4 lookAt(const vec3 & eye, const vec3 & center, const vec3 & up)
{
    return glm::lookAt(eye, center, up);
}

} /* math */
} /* eps */

#endif // MATH_TRANSFORM_H_INCLUDED
