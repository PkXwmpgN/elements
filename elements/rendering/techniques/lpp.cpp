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

#include "lpp.h"
#include "lpp_geometry_pass.h"
#include "lpp_lighting_pass.h"
#include "lpp_reconstruct_pass.h"

namespace eps {
namespace rendering {
namespace techniques {

bool lpp::initialize()
{
    passes_.initialize(3);

    link_geometry_ = passes_.add_pass<lpp_geometry_pass>();
    if(link_geometry_.expired())
        return false;

    link_lighting_ = passes_.add_pass<lpp_lighting_pass>();
    if(link_lighting_.expired())
        return false;

    link_reconstruct_ = passes_.add_pass<lpp_reconstruct_pass>();
    if(link_reconstruct_.expired())
        return false;

    passes_.add_dependency(link_lighting_, link_geometry_, pass_slot::slot_0, pass_slot::slot_0);
    passes_.add_dependency(link_lighting_, link_geometry_, pass_slot::slot_1, pass_slot::slot_1);
    passes_.add_dependency(link_reconstruct_, link_lighting_, pass_slot::slot_0);

    return true;
}

void lpp::set_scene(const utils::pointer<scene::scene> & scene)
{
    if(auto pass = link_geometry_.lock())
        pass->set_scene(scene);
    if(auto pass = link_lighting_.lock())
        pass->set_scene(scene);
    if(auto pass = link_reconstruct_.lock())
        pass->set_scene(scene);
}

void lpp::process(float dt)
{
    passes_.process(dt);
}

utils::unique<pass_target> lpp::construct(const math::uvec2 & size)
{
    passes_.construct(size, construction_flag::depth_stencil);
    return nullptr;
}


} /* techniques */
} /* rendering */
} /* eps */
