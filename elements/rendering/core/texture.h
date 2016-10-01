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

#ifndef RENDERING_TEXTURE_H_INCLUDED
#define RENDERING_TEXTURE_H_INCLUDED

#include "opengl.h"
#include "math/types.h"
#include "utils/std/product.h"

namespace eps {
namespace rendering {

class texture
{
public:

    explicit texture(const math::uvec2 & size);
    texture(const product_type & product, const math::uvec2 & size);

    ~texture();

    texture() = default;
    texture(const texture &) = delete;
    texture & operator=(const texture &) = delete;
    texture(texture &&) = default;
    texture & operator=(texture &&) = default;

    const product_type & get_product() const;
    const math::uvec2 & get_size() const;

    bool valid() const;
    product_type release();

private:

    product_type product_;
    math::uvec2 size_;
};

} /* rendering */
} /* eps */

#endif // RENDERING_TEXTURE_H_INCLUDED
