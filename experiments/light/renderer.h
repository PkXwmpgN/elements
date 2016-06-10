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

#ifndef EXPERIMENTS_LIGHT_RENDERER_H_INCLUDED
#define EXPERIMENTS_LIGHT_RENDERER_H_INCLUDED

#include <elements/synchronization/future.h>
#include <elements/rendering/passes/pass_composition.h>
#include <elements/rendering/effects/forwards.h>

namespace eps {
namespace experiment {
namespace light {

class pass_velocities;
class pass_positions;
class pass_particles;
class pass_occluding;

class renderer
{
public:

    bool initialize();
    bool construct(const math::uvec2 & size, size_t particles_count);
    void render(float dt);

    void set_field(sync::future<math::vec2> field);

    bool set_background(const std::string & asset);
    void set_color(const math::vec3 & color);

private:

    rendering::pass_composition passes_;

    utils::link<pass_velocities> link_velocities_;
    utils::link<pass_positions> link_positions_;
    utils::link<pass_particles> link_particles_;
    utils::link<pass_occluding> link_occluding_;
    utils::link<rendering::effect::light_scattered> link_ls_;
    utils::link<rendering::effect::clear> link_clear_;
};

} /* light */
} /* experiment */
} /* eps */

#endif // EXPERIMENTS_LIGHT_RENDERER_H_INCLUDED
