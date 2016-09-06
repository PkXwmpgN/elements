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

#include "collide.h"

namespace eps {
namespace collision {

bool collide(const aabb & a, const aabb & b)
{
    if(a.max()[0] < b.min()[0] || a.min()[0] > b.max()[0])
        return false;
    if(a.max()[1] < b.min()[1] || a.min()[1] > b.max()[1])
        return false;
    if(a.max()[2] < b.min()[2] || a.min()[2] > b.max()[2])
        return false;
    return true;
}

bool collide(const aabb & a, const aabb & b, contact & cnt)
{
    static math::vec3 normals[] =
    {
        {-1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, 1.0f}
    };

    float distances[] =
    {
        b.max().x - a.min().x,
        a.max().x - b.min().x,
        b.max().y - a.min().y,
        a.max().y - b.min().y,
        b.max().z - a.min().z,
        a.max().z - b.min().z
    };

    for(int i = 0; i < 6; ++i)
    {
        if(distances[i] < 0.0f)
            return false;

        if(i == 0 || distances[i] < cnt.depth)
        {
            cnt.normal = normals[i];
            cnt.depth = distances[i];
        }
    }

    return true;
}

} /* collision */
} /* eps */
