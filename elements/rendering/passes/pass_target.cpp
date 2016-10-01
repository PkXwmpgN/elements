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

#include "pass_target.h"

namespace eps {
namespace rendering {

pass_target_default::pass_target_default(target target)
    : target_(std::move(target))
{}

const product_type & pass_target_default::get_product(const pass_slot & slot) const
{
    if(utils::to_int(slot) < utils::to_int(attachment::MAX))
    {
        static attachment attachments[] =
        {
            attachment::color0,
            attachment::depth
        };
        return target_.get_product(attachments[utils::to_int(slot)]);
    }

    return product_type::default_product();
}

const product_type & pass_target_default::get_target() const
{
    return target_.get_target();
}

const math::uvec2 & pass_target_default::get_target_size() const
{
    return target_.get_size();
}

void pass_target_default::swap() {}

void pass_target_default::attach_depth(texture buffer)
{
    target_.attach(attachment::depth, std::move(buffer));
}

void pass_target_default::attach_stencil(texture buffer)
{
    target_.attach(attachment::stencil, std::move(buffer));
}

} /* rendering */
} /* eps */
