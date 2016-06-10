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

#ifndef RENDERING_PASSES_PASS_TARGET_INPUT_H_INCLUDED
#define RENDERING_PASSES_PASS_TARGET_INPUT_H_INCLUDED

#include "utils/std/pointer.h"
#include "pass_input_slot.h"
#include "pass_target_placement.h"

namespace eps {
namespace rendering {

class pass_target_input
{
public:

    pass_target_input(size_t place, utils::link<pass_target_placement> placement);

    const product_type & get_slot(const pass_input_slot & slot) const;
    const product_type & get_product() const;

private:

    size_t place_;
    utils::link<pass_target_placement> placement_;
};

} /* rendering */
} /* eps */

#endif // RENDERING_PASSES_PASS_TARGET_PLACEMENT_H_INCLUDED
