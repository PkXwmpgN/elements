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

#include "particles_product.h"
#include "pass_positions.h"
#include "pass_particles.h"

#include <elements/rendering/effects/clear.h>
#include <elements/math/common.h>

namespace eps {
namespace experiment {
namespace strange {

void particles_product::set_count(size_t count)
{
    particles_count_ = count;
}

bool particles_product::initialize()
{
    // TODO: from xml + register user types

    passes_.initialize(3);

    auto link_clear = passes_.add_pass<rendering::effect::clear>();
    if(link_clear.expired())
        return false;

    auto link_positions = passes_.add_pass<pass_positions>();
    if(link_positions.expired())
        return false;

    auto link_particles = passes_.add_pass<pass_particles>();
    if(link_particles.expired())
        return false;

    passes_.add_dependency(link_particles, link_positions, rendering::pass_slot::slot_0);

    return true;
}

void particles_product::process(float dt)
{
    passes_.process(dt);
}

utils::unique<rendering::pass_target> particles_product::construct(const math::uvec2 &)
{
    const float length = math::sqrt(particles_count_);
    passes_.construct(math::uvec2(length, length));

    return nullptr;
}

} /* strange */
} /* experiment */
} /* eps */
