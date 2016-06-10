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

#ifndef EXPERIMENTS_LIQUID_PASS_PARTICLES_H_INCLUDED
#define EXPERIMENTS_LIQUID_PASS_PARTICLES_H_INCLUDED

#include <elements/synchronization/future.h>
#include <elements/rendering/core/buffer.h>
#include <elements/rendering/core/program.h>
#include <elements/rendering/passes/pass_base.h>

namespace eps {
namespace experiment {
namespace liquid {

class pass_particles : public rendering::pass_base
{
public:

    using rendering::pass_base::pass_base;

    void set_particles(sync::future<math::vec2> particles);
    void set_particles_transform(const math::mat4 & transform);
    void set_particles_size(float size);

public:

    bool initialize() final;
    void process(float dt) final;

private:

    rendering::vertices vertices_;
    rendering::program program_;

    sync::future<math::vec2> particles_;
    math::mat4 particles_transform_;

    float particles_size_ = 1.0f;
};

} /* liquid */
} /* experiment */
} /* eps */

#endif // EXPERIMENTS_LIQUID_PASS_PARTICLES_H_INCLUDED
