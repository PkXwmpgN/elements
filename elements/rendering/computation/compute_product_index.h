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

#ifndef RENDERING_COMPUTATION_COMPUTE_PRODUCT_INDEX_H_INCLUDED
#define RENDERING_COMPUTATION_COMPUTE_PRODUCT_INDEX_H_INCLUDED

#include "math/types.h"
#include "rendering/core/buffer.h"

namespace eps {
namespace rendering {

class compute_product_index
{
public:

    void construct(const math::uvec2 & size);

    size_t get_product_count() const;
    const product_type & get_product() const;

private:

    vertices vertices_;

    size_t count_ = 0;
};

} /* rendering */
} /* eps */

#endif // RENDERING_COMPUTATION_COMPUTE_PRODUCT_INDEX_H_INCLUDED
