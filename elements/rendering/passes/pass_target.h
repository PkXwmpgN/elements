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

#ifndef RENDERING_PASSES_PASS_TARGET_H_INCLUDED
#define RENDERING_PASSES_PASS_TARGET_H_INCLUDED

#include "rendering/core/target.h"
#include "rendering/core/target_maker.h"
#include "utils/std/pointer.h"
#include "pass_slot.h"

namespace eps {
namespace rendering {

class pass_target
{
public:

    virtual const product_type & get_product(const pass_slot & slot) const = 0;
    virtual const product_type & get_target() const = 0;
    virtual const math::uvec2 &  get_target_size() const = 0;
    virtual void swap() = 0;

    virtual ~pass_target() {}
};

class pass_target_default : public pass_target
{
public:

    explicit pass_target_default(target target);

    const product_type & get_product(const pass_slot & slot) const final;
    const product_type & get_target() const final;
    const math::uvec2 &  get_target_size() const final;

    void swap() final;

private:

    target target_;
};

template<typename... _Policies>
inline utils::unique<pass_target> get_pass_target(const math::uvec2 & size)
{
    auto maker = get_target_maker<_Policies...>();
    return utils::make_unique<pass_target_default>(maker.construct(size));
}

} /* renderer */
} /* eps */

#endif // RENDERING_PASSES_PASS_TARGET_H_INCLUDED
