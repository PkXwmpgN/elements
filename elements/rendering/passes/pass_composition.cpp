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

#include "pass_composition.h"
#include "pass_base.h"
#include "rendering/state/state_macro.h"
#include "rendering/core/texture_policy.h"

namespace eps {
namespace rendering {

pass_composition::pass_composition()
    : placement_(utils::make_shared<pass_target_placement>())
{}

void pass_composition::initialize(size_t places_count)
{
    placement_->initialize(places_count);
}

void pass_composition::construct(const math::uvec2 & size, construction_flag flag)
{
    placement_->clear_targets();

    product_type shared_depth;
    product_type shared_stencil;

    if(flag == construction_flag::depth)
    {
        auto maker = get_texture_maker<depth24_texture_policy>();
        shared_depth = maker.construct(nullptr, size).release();
    }
    else if(flag == construction_flag::depth_stencil)
    {
        auto maker = get_texture_maker<depth24_stencil8_texture_policy>();
        auto product = maker.construct(nullptr, size).release();
        shared_depth = product;
        shared_stencil = product;
    }

    for(auto & pass : passes_)
        placement_->register_target(pass->get_place(), pass->construct(size));

    placement_->construct(size, shared_depth, shared_stencil);
}

void pass_composition::process(float dt)
{
    for(auto & pass : passes_)
    {
        if(auto output = placement_->get_target(pass->get_place()).lock())
        {
            EPS_STATE_VIEWPORT(output->get_target_size());
            EPS_STATE_TARGET(output->get_target());
            pass->process(dt);
            output->swap();
        }
        else
        {
            pass->process(dt);
        }
    }
}

} /* rendering */
} /* eps */
