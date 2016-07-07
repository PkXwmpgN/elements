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

#include "pass_particles.h"
#include <elements/rendering/state/state_macro.h>
#include <elements/rendering/utils/program_loader.h>
#include <elements/utils/std/enum.h>

namespace eps {
namespace experiment {
namespace liquid {

enum class program_enum : short
{
    // attributes
    a_vertex_xy = 0,
    // uniforms
    u_transform = 0,
    u_size = 1
};

void pass_particles::set_particles(sync::future<math::vec2> particles)
{
    particles_ = std::move(particles);
}

void pass_particles::set_particles_transform(const math::mat4 & transform)
{
    particles_transform_ = transform;
}

void pass_particles::set_particles_size(float size)
{
    particles_size_ = size;
}

bool pass_particles::initialize()
{
    return rendering::load_program("assets/shaders/experiments/liquid/particles.prog", program_);
}

void pass_particles::process(float)
{
    if(particles_.valid())
    {
        particles_.wait();
        vertices_.allocate(particles_.output().data(),
                           particles_.output().size(), sizeof(math::vec2));

        EPS_STATE_VERTICES(vertices_.get_product());
        EPS_STATE_PROGRAM(program_.get_product());

        program_.attribute_array(utils::to_int(program_enum::a_vertex_xy), 0, 2, 0);
        program_.attribute_array_enable(utils::to_int(program_enum::a_vertex_xy));
        program_.uniform_value(utils::to_int(program_enum::u_transform), particles_transform_);
        program_.uniform_value(utils::to_int(program_enum::u_size), particles_size_);

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_POINTS, 0, particles_.output().size());
    }
}

} /* liquid */
} /* experiment */
} /* eps */
