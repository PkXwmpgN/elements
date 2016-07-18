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

#include "renderer.h"
#include "pass_character.h"

#include <elements/rendering/effects/clear.h>

namespace eps {
namespace experiment {
namespace character {

bool renderer::initialize()
{
    using namespace rendering;

    passes_.initialize(2);

    auto link_clear = passes_.add_pass<effect::clear>();
    if(link_clear.expired())
        return false;

    link_character_ = passes_.add_pass<pass_character>();
    if(link_character_.expired())
        return false;

    link_clear.lock()->set_color(math::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    link_clear.lock()->set_bits(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return true;
}

bool renderer::construct(const math::uvec2 & size)
{
    passes_.construct(size);
    return true;
}

void renderer::render(float dt)
{
    passes_.process(dt);
}

bool renderer::set_model(const std::string & model)
{
    if(auto link = link_character_.lock())
        return link->set_model(model);
    return false;
}

void renderer::set_rotation(float theta, float phi)
{
    if(auto link = link_character_.lock())
        link->set_rotation(theta, phi);
}

} /* character */
} /* experiment */
} /* eps */
