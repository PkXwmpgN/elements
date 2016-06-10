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

#ifndef MATH_COMMON_H_INCLUDED
#define MATH_COMMON_H_INCLUDED

#include "types.h"

namespace eps {
namespace math {

inline float sqrt(float a)
{
    return glm::sqrt(a);
}

inline float ceil(float a)
{
    return glm::ceil(a);
}

inline float floor(float a)
{
    return glm::floor(a);
}

inline float fract(float a)
{
    return glm::fract(a);
}

inline bool equal(float a, float b)
{
    return glm::abs(a - b) < 0.0001f;
}

inline float abs(float a)
{
    return glm::abs(a);
}

inline float clamp(float v, float min, float max)
{
    return glm::clamp(v, min, max);
}

} /* math */
} /* eps */

#endif // MATH_COMMON_H_INCLUDED
