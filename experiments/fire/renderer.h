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

#ifndef EXPERIMENTS_FIRE_RENDERER_H_INCLUDED
#define EXPERIMENTS_FIRE_RENDERER_H_INCLUDED

#include <elements/rendering/passes/pass_composition.h>
#include <elements/rendering/effects/forwards.h>

namespace eps {
namespace experiment {
namespace fire {

class pass_raymarching;
class pass_fire;

class renderer
{
public:

    bool initialize();
    bool construct(const math::uvec2 & size, size_t texture_size);

    void render(float dt);

    bool set_background(const char * asset);

    void set_intensity(float intensity);
    void set_covering(float covering);
    void set_color_hot(const math::vec3 & color);
    void set_color_cold(const math::vec3 & color);

private:

    rendering::pass_composition passes_;

    utils::link<pass_raymarching> link_raymarching_;
    utils::link<pass_fire> link_fire_;
    utils::link<rendering::effect::blur> link_blur_;

    math::uvec2 size_reduction_;
};

} /* fire */
} /* experiment */
} /* eps */

#endif // EXPERIMENTS_FIRE_RENDERER_H_INCLUDED
