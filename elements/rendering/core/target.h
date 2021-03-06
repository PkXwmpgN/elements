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

#ifndef RENDERING_TARGET_H_INCLUDED
#define RENDERING_TARGET_H_INCLUDED

#include "texture.h"
#include "target_attachment.h"
#include "utils/std/enum.h"
#include <array>

namespace eps {
namespace rendering {

class target
{
public:

    target();
    ~target();

    target(const target &) = delete;
    target & operator=(const target &) = delete;
    target(target &&) = default;
    target & operator=(target &&) = default;

    bool attach(attachment id, texture data);

    const product_type & get_target() const;

    const product_type & get_product() const;
    const product_type & get_product(attachment id) const;

    const math::uvec2 & get_size() const;
    const math::uvec2 & get_size(attachment id) const;

private:

    product_type product_;
    std::array<texture, utils::to_int(attachment::MAX)> attachments_;
};

} /* rendering */
} /* eps */

#endif // RENDERING_TARGET_H_INCLUDED
