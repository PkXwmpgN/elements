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

#ifndef SIMULATION_CONFIG_H_INCLUDED
#define SIMULATION_CONFIG_H_INCLUDED

#include "math/types.h"
#include <string>

namespace eps {
namespace simulation {
namespace liquid {

class config
{
public:

    config();

    float density() const { return density_; }
    void  density(float value) { density_ = value;}

    float pressure() const { return pressure_; }
    float pressure_near() const { return pressure_near_; }
    void  pressure(float value, float value_near)
    {
        pressure_ = value;
        pressure_near_ = value_near;
    }

    float spring() const { return spring_; }
    void  spring(float value) { spring_ = value; }

    float viscosty_sigma() const { return viscosity_sigma_; }
    float viscosty_beta() const { return viscosity_beta_; }
    void  viscosty(float sigma, float beta)
    {
        viscosity_sigma_ = sigma;
        viscosity_beta_ = beta;
    }

    const math::vec2 & gravity() const { return gravity_; }
    void gravity(const math::vec2 & gravity) { gravity_ = gravity; }

private:

    // density
    float density_;
    // pressure
    float pressure_;
    float pressure_near_;
    // spring, elasticity
    float spring_;
    // viscosity
    float viscosity_sigma_;
    float viscosity_beta_;

    // gravity
    math::vec2 gravity_;
};

} /* liquid */
} /* simulation */
} /* eps */


#endif //SIMULATION_CONFIG_H_INCLUDED
