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

#include "compute_product_index.h"
#include <vector>

namespace eps {
namespace rendering {

size_t compute_product_index::get_product_count() const
{
    return count_;
}

const product_type & compute_product_index::get_product() const
{
    return vertices_.get_product();
}

void compute_product_index::construct(const math::uvec2 & size)
{
    count_ = size.x * size.y;
    std::vector<math::vec2> uvs(count_);

    const float s = 1.0f / float(size.x);
    const float t = 1.0f / float(size.y);

    for(size_t i = 0; i < uvs.size(); ++i)
    {
        uvs[i].x = (i % size.x + 0.5f) * s;
        uvs[i].y = (i / size.x + 0.5f) * t;
    }

    vertices_.allocate(uvs.data(), uvs.size(), sizeof(math::vec2));
}

} /* rendering */
} /* eps */
