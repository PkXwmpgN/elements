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

#ifndef EXPERIMENTS_STRANGE_RENDERER_H_INCLUDED
#define EXPERIMENTS_STRANGE_RENDERER_H_INCLUDED

#include <elements/utils/std/pointer.h>
#include <elements/rendering/effects/forwards.h>
#include <elements/rendering/passes/pass_composition.h>

namespace eps {
namespace experiment {
namespace strange {

class renderer
{
public:

    bool initialize(size_t particles_count);
    bool construct(const math::uvec2 & size);

    void render(float dt);

    void color_background(const math::vec3 & color);
    void color_gradient(const math::vec3 & color_1, const math::vec3 & color_2);

private:

    rendering::pass_composition passes_;

    utils::link<rendering::effect::gradient> link_gradient_;
    utils::link<rendering::effect::clear> link_clear_;
};

} /* strange */
} /* experiment */
} /* eps */

#endif // EXPERIMENTS_STRANGE_RENDERER_H_INCLUDED
