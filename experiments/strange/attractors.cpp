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

#include "attractors.h"

#include <elements/math/constants.h>
#include <elements/math/common.h>

namespace eps {
namespace experiment {
namespace strange {

void attractors::data::update(float next)
{
    target = next < 0.0f ? math::clamp(next, -0.5f, -math::pi<float>() * 0.6f)
                         : math::clamp(next,  0.5f,  math::pi<float>() * 0.6f);
    inc = target - value;
}

attractors::attractors()
    : distribution_(-math::pi<float>(), math::pi<float>())
    , generator_(std::random_device{}())
{
    for(auto & data : attractors_)
    {
        data.value = distribution_(generator_);
        data.update(distribution_(generator_));
    }
}

void attractors::process(float dt)
{
    for(auto & data : attractors_)
    {
        data.value += data.inc * dt * 0.2f;
        if(math::abs(data.target - data.value) < math::abs(data.inc * dt))
            data.update(distribution_(generator_));
    }
}

} /* strange */
} /* experiment */
} /* eps */
