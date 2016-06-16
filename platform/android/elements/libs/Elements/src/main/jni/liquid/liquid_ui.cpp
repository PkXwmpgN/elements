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

#include "liquid_ui.h"
#include "logging.h"
#include <elements/math/common.h>


// ------------------------------------------
// viscosity

viscosity_model::viscosity_model(eps::utils::link<eps::simulation::liquid::system> liquid)
    : liquid_(liquid)
    , value_(config_.load_sigma_normalize())
{
    eps::preferences::instance().register_listener(this);
}

viscosity_model::~viscosity_model()
{
    eps::preferences::instance().unregister_listener(this);
}

float viscosity_model::get_value() const
{
    return value_;
}

void viscosity_model::set_value(float value)
{
    const float new_value = eps::math::floor(value / 0.1f) * 0.1f;
    if(!eps::math::equal(new_value, value_))
    {
        config_.save_sigma_normalize(new_value);
        config_.save_sigma_normalize(new_value);
    }
}

void viscosity_model::on_changed(const std::string & key, float value)
{
    if(liquid_.expired())
    {
        LOGE("[Liquid] viscisity changed faled: simulation is expired");
        return;
    }
    if(key == config_.key_sigma())
    {
        value_ = value;
        liquid_.lock()->set_viscosity(config_.prepare_sigma(value_),
                                      config_.prepare_beta(value_));
    }
}

// --------------------------------------------
// density

density_model::density_model(eps::utils::link<eps::simulation::liquid::system> liquid)
    : liquid_(liquid)
    , value_(config_.load_normalize())
{
    eps::preferences::instance().register_listener(this);
}

density_model::~density_model()
{
    eps::preferences::instance().unregister_listener(this);
}

float density_model::get_value() const
{
    return value_;
}

void density_model::set_value(float value)
{
    const float new_value = eps::math::floor(value / 0.1f) * 0.1f;
    if(!eps::math::equal(new_value, value_))
        config_.save_normalize(new_value);
}

void density_model::on_changed(const std::string &key, float value)
{
    if(liquid_.expired())
    {
        LOGE("[Liquid] density changed faled: simulation is expired");
        return;
    }

    if(key == config_.key())
    {
        value_ = value;
        liquid_.lock()->set_density(config_.prepare(value_));
    }
}

// --------------------------------------------
// pressure
pressure_model::pressure_model(eps::utils::link<eps::simulation::liquid::system> liquid)
    : liquid_(liquid)
    , value_(config_.load_normalize())
{
    eps::preferences::instance().register_listener(this);
}

pressure_model::~pressure_model()
{
    eps::preferences::instance().unregister_listener(this);
}

float pressure_model::get_value() const
{
    return value_;
}

void pressure_model::set_value(float value)
{
    const float new_value = eps::math::floor(value / 0.1f) * 0.1f;
    if(!eps::math::equal(new_value, value_))
    {
        config_.save_normalize(new_value);
        config_.save_near_normalize(new_value);
    }
}

void pressure_model::on_changed(const std::string & key, float value)
{
    if(liquid_.expired())
    {
        LOGE("[Liquid] pressure changed faled: simulation is expired");
        return;
    }

    if(key == config_.key())
    {
        value_ = value;
        liquid_.lock()->set_pressure(config_.prepare(value_),
                                     config_.prepare_near(value_));
    }
}

// --------------------------------------------
// elastisity

elasticity_model::elasticity_model(eps::utils::link<eps::simulation::liquid::system> liquid)
    : liquid_(liquid)
    , value_(config_.load_normalize())
{
    eps::preferences::instance().register_listener(this);
}

elasticity_model::~elasticity_model()
{
    eps::preferences::instance().unregister_listener(this);
}

float elasticity_model::get_value() const
{
    return value_;
}

void elasticity_model::set_value(float value)
{
    const float new_value = eps::math::floor(value / 0.1f) * 0.1f;
    if(!eps::math::equal(new_value, value_))
        config_.save_normalize(new_value);
}

void elasticity_model::on_changed(const std::string &key, float value)
{
    if(liquid_.expired())
    {
        LOGE("[Liquid] elasticity changed faled: simulation is expired");
        return;
    }

    if(key == config_.key())
    {
        value_ = value;
        liquid_.lock()->set_elasticity(config_.prepare(value_));
    }
}
