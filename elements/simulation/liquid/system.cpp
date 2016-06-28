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

#include "system.h"

#include "math/common.h"
#include "math/geometric.h"

#include "assets/assets_storage.h"

#include <thread>
#include <random>
#include <vector>
#include <algorithm>

namespace eps {
namespace simulation {
namespace liquid {

bool system::construct(const math::vec2 & size, size_t particle_count)
{
    if(volume_.construct(size, particle_count))
    {
        init_particles_distribution();
        return true;
    }
    return false;
}

void system::init_particles_distribution()
{
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<float> wdistribution(0.0f, volume_.get_size().x);
    std::uniform_real_distribution<float> hdistribution(0.0f, volume_.get_size().y);

    auto range = volume_.get_particles_range();
    std::generate(range.begin(), range.end(), [&]()
    {
        return particle
        {
            wdistribution(generator),
            hdistribution(generator)
        };
    });
}

sync::task<math::vec2>::future system::spawn(float dt)
{
    const size_t size = volume_.get_particles_range().size();
    return simulation_task_.start(size, [this](auto begin, auto /*end*/, float dt)
    {
         config cfg = config_;

         // neighbors search
         volume_.partition();

         // apply gravity
         apply_gravity(cfg, dt);

         // modify velocities with pairwise viscosity impulses
         apply_viscosity(cfg, dt);

         // save previous position.
         // advance to predicted position
         predict_position(cfg, dt);

         // double density relaxation;
         double_density_relaxation(cfg, dt);

         // resolve collisions
         resolve_collisions(cfg, dt);

         // use previous position to compute next velocity
         next_velocity(cfg, dt);

         for(auto & particle : volume_.get_particles_range())
         {
             *begin = particle.pos;
             ++begin;
         }

    }, dt);
}

void system::touch(const math::vec2 & pos, float radius)
{
    touch_ = obstacle(pos, radius * volume_.get_distance());
}

void system::apply_gravity(const config & cfg, float dt)
{
    for(auto & particle : volume_.get_particles_range())
        particle.velocity += cfg.gravity() * dt;
}

void system::apply_viscosity(const config & cfg, float dt)
{
    auto prange = volume_.get_particles_range();
    for(auto particle = prange.begin(); particle != prange.end(); ++particle)
    {
        auto particle_index = std::distance(prange.begin(), particle);
        for(size_t i = 0; i < volume_.get_neighbors_range_count(); ++i)
        {
            auto nrange = volume_.get_neighbors_range(particle_index, i);
            for(auto neighbor = nrange.begin(); neighbor != nrange.end(); ++neighbor)
            {
                if(neighbor == particle)
                    continue;

                const float distance2 = math::distance2(particle->pos, neighbor->pos);
                if(distance2 < volume_.get_distance2())
                {
                    const float distance = math::sqrt(distance2);
                    if(distance > 0.0001)
                    {
                        const math::vec2 rn = (particle->pos - neighbor->pos) / distance;

                        // inward radial velocity
                        const float u = math::dot(particle->velocity - neighbor->velocity, rn);
                        if(u > 0.0f)
                        {
                            // linear and quadratic impulses
                            const math::vec2 impulse =  0.5f * rn * dt *
                                    (1.0f - distance / volume_.get_distance()) *
                                    (cfg.viscosty_sigma() * u + cfg.viscosty_beta() * u * u);

                            neighbor->velocity += impulse;
                            particle->velocity -= impulse;
                        }
                    }
                }
            }
        }
    }
}

void system::predict_position(const config &, float dt)
{
    for(auto & particle : volume_.get_particles_range())
    {
        particle.prev = particle.pos;
        particle.pos += particle.velocity * dt;
    }
}

void system::double_density_relaxation(const config & cfg, float dt)
{
    auto prange = volume_.get_particles_range();
    for(auto particle = prange.begin(); particle != prange.end(); ++particle)
    {
        float density = 0;
        float density_near = 0;

        // compute density and near-density
        auto particle_index = std::distance(prange.begin(), particle);
        for(size_t i = 0; i < volume_.get_neighbors_range_count(); ++i)
        {
            auto nrange = volume_.get_neighbors_range(particle_index, i);
            for(auto neighbor = nrange.begin(); neighbor != nrange.end(); ++neighbor)
            {
                if(neighbor == particle)
                    continue;
                const float distance2 = math::distance2(particle->pos, neighbor->pos);
                if(distance2 < volume_.get_distance2())
                {
                    const float q = math::sqrt(distance2) / volume_.get_distance();
                    if(q < 1.0f)
                    {
                        const float dp2 = (1.0f - q) * (1.0f - q);
                        density += dp2;
                        density_near += dp2 * (1.0f - q);
                    }
                }
            }
        }

        // compute pressure and near-pressure
        const float pressure = cfg.pressure() * (density - cfg.density());
        const float pressure_near = cfg.pressure_near() * density_near;

        // apply displacements
        for(size_t i = 0; i < volume_.get_neighbors_range_count(); ++i)
        {
            auto nrange = volume_.get_neighbors_range(particle_index, i);
            for(auto neighbor = nrange.begin(); neighbor != nrange.end(); ++neighbor)
            {
                if(neighbor == particle)
                    continue;

                const float distance2 = math::distance2(particle->pos, neighbor->pos);
                if(distance2 < volume_.get_distance2())
                {
                    const float distance = math::sqrt(distance2);
                    if(distance > 0.0001)
                    {
                        const float q = 1.0f - distance / volume_.get_distance();
                        const math::vec2 rn = (neighbor->pos - particle->pos) / distance;
                        const math::vec2 D = rn * dt * dt *
                                (pressure * q + pressure_near * q * q +
                                 cfg.spring() * (volume_.get_distance() - distance) * q) * 0.5f;

                        neighbor->pos += D;
                        particle->pos -= D;
                    }
                }
            }
        }
    }
}

void system::resolve_collisions(const config &, float)
{
    for(auto & particle : volume_.get_particles_range())
    {
        if(touch_.radius() > 0.001f)
        {
            const float distance = math::distance(particle.pos, touch_.position());
            if(distance < touch_.radius())
            {
                const math::vec2 rn = (touch_.position() - particle.pos) / distance;
                const math::vec2 delta = (touch_.radius() - distance) * rn;
                particle.pos -= delta;
                particle.prev -= delta;
            }
        }

        particle.pos.x = math::clamp(particle.pos.x, 0.0f, volume_.get_size().x);
        particle.pos.y = math::clamp(particle.pos.y, 0.0f, volume_.get_size().y);
    }
}

void system::next_velocity(const config &, float dt)
{
    for(auto & particle : volume_.get_particles_range())
        particle.velocity = (particle.pos - particle.prev) / dt;
}

} /* liquid */
} /* simulation */
} /* eps */
