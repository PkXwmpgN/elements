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

#ifndef COLLISTION_AABB_H_INCLUDED
#define COLLISTION_AABB_H_INCLUDED

#include "math/types.h"

namespace eps {
namespace collision {

struct aabb
{
    aabb() = default;
    aabb(const math::vec3 & min, const math::vec3 & max);

    const math::vec3 & min() const { return min_; }
    const math::vec3 & max() const { return max_; }
    const math::vec3 & center() const { return center_; }
    const math::vec3 & size() const { return size_; }

private:

    math::vec3 min_;
    math::vec3 max_;
    math::vec3 center_;
    math::vec3 size_;
};

aabb update(const aabb & bounds, const math::mat4 & m);

} /* collision */
} /* eps */

#endif // COLLISTION_AABB_H_INCLUDED
