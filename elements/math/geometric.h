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

#ifndef MATH_UTILS_H_INCLUDED
#define MATH_UTILS_H_INCLUDED

#include "types.h"

namespace eps {
namespace math {

inline float distance(const vec2 & a, const vec2 & b)
{
    return glm::distance(a, b);
}

inline float distance2(const vec2 & a, const vec2 & b)
{
    const vec2 t = a - b;
    return glm::dot(t, t);
}

inline float length(const vec2 & a)
{
    return glm::length(a);
}

inline float dot(const vec2 & a, const vec2 & b)
{
    return glm::dot(a, b);
}

inline vec2 normalize(const vec2 & a)
{
    return glm::normalize(a);
}

} /* math */
} /* eps */

#endif // MATH_UTILS_H_INCLUDED
