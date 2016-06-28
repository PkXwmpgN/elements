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

#ifndef SIMULATION_LIQUID_SYSTEM_H_INCLUDED
#define SIMULATION_LIQUID_SYSTEM_H_INCLUDED

#include "math/types.h"
#include "synchronization/future.h"
#include "synchronization/task.h"

#include "config.h"
#include "volume.h"
#include "obstacle.h"

namespace eps {
namespace simulation {
namespace liquid {

class system
{
public:

    system() = default;
    system(const system &) = delete;
    system & operator=(const system &) = delete;

    bool construct(const math::vec2 & size, size_t particle_count);
    void touch(const math::vec2 & pos, float radius);

    sync::task<math::vec2>::future spawn(float dt);

    void set_gravity(const math::vec2 & value) { config_.gravity(value); }
    void set_density(float value) { config_.density(value); }
    void set_viscosity(float sigma, float beta) { config_.viscosty(sigma, beta); }
    void set_pressure(float value, float near) { config_.pressure(value, near); }
    void set_elasticity(float value) { config_.spring(value); }

private:

    void init_particles_distribution();

    void apply_gravity(const config & cfg, float dt);
    void apply_viscosity(const config & cfg, float dt);
    void predict_position(const config & cfg, float dt);
    void double_density_relaxation(const config & cfg, float dt);
    void resolve_collisions(const config & cfg, float dt);
    void next_velocity(const config & cfg, float dt);

private:

    volume volume_;
    config config_;
    obstacle touch_;

    sync::task<math::vec2> simulation_task_;
};

} /* liquid */
} /* simulation */
} /* eps */


#endif // SIMULATION_LIQUID_SYSTEM_H_INCLUDED
