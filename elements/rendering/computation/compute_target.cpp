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

#include "compute_target.h"
#include "rendering/core/texture_policy.h"
#include "rendering/core/target_maker.h"

namespace eps {
namespace rendering {

compute_target::compute_target(const math::uvec2 & size)
{
    auto maker = get_target_maker<default_texture_policy>();
    target_ = maker.construct_buffered(size);
}

const product_type & compute_target::get_product() const
{
    return target_.get_product();
}

const product_type & compute_target::get_target() const
{
    return target_.get_target();
}

const math::uvec2 & compute_target::get_size() const
{
    return target_.get_size();
}

void compute_target::swap()
{
    target_.swap();
}

} /* rendering */
} /* eps */
