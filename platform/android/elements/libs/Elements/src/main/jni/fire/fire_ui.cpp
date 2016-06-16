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

#include "fire_ui.h"
#include "logging.h"
#include <elements/math/common.h>

// ------------------------------------------
// intensity

fire_intensity_model::fire_intensity_model(eps::utils::link<eps::experiment::fire::renderer> fire)
    : fire_(fire)
    , value_(config_.load_normalize())
{
    eps::preferences::instance().register_listener(this);
}

fire_intensity_model::~fire_intensity_model()
{
    eps::preferences::instance().unregister_listener(this);
}

float fire_intensity_model::get_value() const
{
    return value_;
}

void fire_intensity_model::set_value(float value)
{
    const float new_value = eps::math::floor(value / 0.1f) * 0.1f;
    if(!eps::math::equal(new_value, value_))
    {
        config_.save_normalize(new_value);
    }
}

void fire_intensity_model::on_changed(const std::string & key, float value)
{
    if(fire_.expired())
    {
        LOGE("[Fire] intensity changed faled: renderer is expired");
        return;
    }

    if(key == config_.key())
    {
        value_ = value;
        fire_.lock()->set_intensity(value_);
    }
}

// ------------------------------------------
// covering

fire_covering_model::fire_covering_model(eps::utils::link<eps::experiment::fire::renderer> fire)
    : fire_(fire)
    , value_(config_.load_normalize())
{
    eps::preferences::instance().register_listener(this);
}

fire_covering_model::~fire_covering_model()
{
    eps::preferences::instance().unregister_listener(this);
}

float fire_covering_model::get_value() const
{
    return value_;
}

void fire_covering_model::set_value(float value)
{
    const float new_value = eps::math::floor(value / 0.1f) * 0.1f;
    if(!eps::math::equal(new_value, value_))
    {
        config_.save_normalize(new_value);
    }
}

void fire_covering_model::on_changed(const std::string & key, float value)
{
    if(fire_.expired())
    {
        LOGE("[Fire] covering changed faled: renderer is expired");
        return;
    }

    if(key == config_.key())
    {
        value_ = value;
        fire_.lock()->set_covering(value_);
    }
}
