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

#ifndef EXPERIMENTS_FIRE_PASS_RAYMARCHING_H_INCLUDED
#define EXPERIMENTS_FIRE_PASS_RAYMARCHING_H_INCLUDED

#include <elements/rendering/core/program.h>
#include <elements/rendering/passes/pass_base.h>
#include <elements/rendering/primitives/square.h>

namespace eps {
namespace experiment {
namespace fire {

class pass_raymarching : public rendering::pass_base
{
public:

    using rendering::pass_base::pass_base;

    void set_intensity(float intensity);
    void set_covering(float covering);
    void set_color_hot(const math::vec3 & color);
    void set_color_cold(const math::vec3 & color);

public:

    bool initialize() final;
    void process(float dt) final;

    utils::unique<rendering::pass_target> construct(const math::uvec2 & size) final;

private:

    rendering::program program_;
    rendering::texture noise_;
    rendering::primitive::square square_;

    math::vec2 texel_;

    math::vec3 color_hot_;
    math::vec3 color_cold_;

    float fire_intensity_ = 0.5f;
    float fire_covering_  = 1.0f;
};

} /* fire */
} /* experiment */
} /* eps */

#endif // EXPERIMENTS_FIRE_PASS_RAYMARCHING_H_INCLUDED
