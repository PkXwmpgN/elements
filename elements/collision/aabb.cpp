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

#include "aabb.h"

namespace eps {
namespace collision {

aabb::aabb(const math::vec3 & min, const math::vec3 & max)
    : min_(min)
    , max_(max)
    , center_((min + max) * 0.5f)
    , size_(max - min)
{}

aabb update(const aabb & bounds, const math::mat4 & m)
{
    math::vec3 min;
    math::vec3 max;
    float e, f;
    for(size_t i = 0; i < 3; ++i)
    {
        min[i] = max[i] = m[3][i];
        for(size_t j = 0; j < 3; ++j)
        {
            e = m[j][i] * bounds.min()[j];
            f = m[j][i] * bounds.max()[j];
            if(e < f)
            {
                min[i] += e;
                max[i] += f;
            }
            else
            {
                min[i] += f;
                max[i] += e;
            }
        }
    }
    return aabb(min, max);
}

} /* collision */
} /* eps */
