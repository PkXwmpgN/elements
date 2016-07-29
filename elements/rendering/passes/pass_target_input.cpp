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

#include "pass_target_input.h"

namespace eps {
namespace rendering {

pass_target_input::pass_target_input(size_t place,
                                     utils::link<pass_target_placement> placement)
    : place_(place)
    , placement_(placement)
{}

const product_type & pass_target_input::get_slot(const pass_slot & slot) const
{
    if(auto placement_link = placement_.lock())
    {
        size_t dependency = placement_link->get_dependency(place_, slot);
        if(auto link = placement_link->get_target(dependency).lock())
        {
            pass_slot output = static_cast<pass_slot>(placement_link->get_dependency_slot(place_, slot));
            return link->get_product(output);
        }
    }

    return product_type::default_product();
}

const product_type & pass_target_input::get_product(const pass_slot & slot) const
{
    if(auto placement_link = placement_.lock())
        if(auto link = placement_link->get_target(place_).lock())
            return link->get_product(slot);

    return product_type::default_product();
}

} /* rendering */
} /* eps */
