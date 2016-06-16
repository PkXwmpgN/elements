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

#include "liquid_config.h"
#include "logging.h"
#include <elements/preferences/preferences.h>

static inline float to_real(float min, float max, float value)
{
    return (max - min) * value + min;
}

static inline float to_normalize(float min, float max, float value)
{
    return (value - min) / (max - min);
}

// -----------------------
// density
const float LIQUID_DENSITY_DEFAULT = 30.0f;
const float LIQUID_DENSITY_MIN     = 0.0f;
const float LIQUID_DENSITY_MAX     = 50.0f;

// -----------------------
// viscosity
const float LIQUID_VISCOSITY_SIGMA_DEFAULT = 1.0f;
const float LIQUID_VISCOSITY_SIGMA_MIN     = 0.0f;
const float LIQUID_VISCOSITY_SIGMA_MAX     = 10.0f;
const float LIQUID_VISCOSITY_BETA_DEFAULT  = 0.1f;
const float LIQUID_VISCOSITY_BETA_MIN      = 0.0f;
const float LIQUID_VISCOSITY_BETA_MAX      = 1.0f;

// -----------------------
// pressure
const float LIQUID_PRESSURE_DEFAULT      = 0.3f;
const float LIQUID_PRESSURE_MIN          = 0.1f;
const float LIQUID_PRESSURE_MAX          = 1.1f;
const float LIQUID_PRESSURE_NEAR_DEFAULT = 6.0f;
const float LIQUID_PRESSURE_NEAR_MIN     = 1.0f;
const float LIQUID_PRESSURE_NEAR_MAX     = 21.0f;

// elasticity
const float LIQUID_ELASTICITY_DEFAULT = 10.0f;
const float LIQUID_ELASTICITY_MIN     = 0.0f;
const float LIQUID_ELASTICITY_MAX     = 100.0f;


// -------------------------------
// density

float density_config::load_normalize() const
{
    return eps::preferences::instance().get_float(key(),
                to_normalize(LIQUID_DENSITY_MIN,
                             LIQUID_DENSITY_MAX,
                             LIQUID_DENSITY_DEFAULT));
}

void density_config::save_normalize(float value) const
{
    eps::preferences::instance().set_float(key(), value);
}

void density_config::save_normalize_default() const
{
    eps::preferences::instance().set_float(key(),
                to_normalize(LIQUID_DENSITY_MIN,
                             LIQUID_DENSITY_MAX,
                             LIQUID_DENSITY_DEFAULT));
}

float density_config::prepare(float value) const
{
    return to_real(LIQUID_DENSITY_MIN,
                   LIQUID_DENSITY_MAX,
                   value);
}

const char * density_config::key() const
{
    return "liquid_density";
}

// --------------------------------
// viscosity

float viscosity_config::load_sigma_normalize() const
{
    return eps::preferences::instance().get_float(key_sigma(),
                to_normalize(LIQUID_VISCOSITY_SIGMA_MIN,
                             LIQUID_VISCOSITY_SIGMA_MAX,
                             LIQUID_VISCOSITY_SIGMA_DEFAULT));
}

void viscosity_config::save_sigma_normalize(float value) const
{
    eps::preferences::instance().set_float(key_sigma(), value);
}

void viscosity_config::save_sigma_normalize_default() const
{
    eps::preferences::instance().set_float(key_sigma(),
                to_normalize(LIQUID_VISCOSITY_SIGMA_MIN,
                             LIQUID_VISCOSITY_SIGMA_MAX,
                             LIQUID_VISCOSITY_SIGMA_DEFAULT));
}

float viscosity_config::prepare_sigma(float value) const
{
    return to_real(LIQUID_VISCOSITY_SIGMA_MIN,
                   LIQUID_VISCOSITY_SIGMA_MAX,
                   value);
}

const char * viscosity_config::key_sigma() const
{
    return "liquid_viscosity_sigma";
}

float viscosity_config::load_beta_normalize() const
{
    return eps::preferences::instance().get_float(key_beta(),
                to_normalize(LIQUID_VISCOSITY_BETA_MIN,
                             LIQUID_VISCOSITY_BETA_MAX,
                             LIQUID_VISCOSITY_BETA_DEFAULT));
}

void viscosity_config::save_beta_normalize(float value) const
{
    eps::preferences::instance().set_float(key_beta(), value);
}

void viscosity_config::save_beta_normalize_default() const
{
    eps::preferences::instance().set_float(key_beta(),
                to_normalize(LIQUID_VISCOSITY_BETA_MIN,
                             LIQUID_VISCOSITY_BETA_MAX,
                             LIQUID_VISCOSITY_BETA_DEFAULT));
}

float viscosity_config::prepare_beta(float value) const
{
    return to_real(LIQUID_VISCOSITY_BETA_MIN,
                   LIQUID_VISCOSITY_BETA_MAX,
                   value);
}

const char * viscosity_config::key_beta() const
{
    return "liquid_viscosity_beta";
}

// --------------------------------
// pressure

float pressure_config::load_normalize() const
{
    return eps::preferences::instance().get_float(key(),
                to_normalize(LIQUID_PRESSURE_MIN,
                             LIQUID_PRESSURE_MAX,
                             LIQUID_PRESSURE_DEFAULT));
}

void pressure_config::save_normalize(float value) const
{
    eps::preferences::instance().set_float(key(), value);
}

void pressure_config::save_normalize_default() const
{
    eps::preferences::instance().set_float(key(),
                to_normalize(LIQUID_PRESSURE_MIN,
                             LIQUID_PRESSURE_MAX,
                             LIQUID_PRESSURE_DEFAULT));
}

float pressure_config::prepare(float value) const
{
    return to_real(LIQUID_PRESSURE_MIN,
                   LIQUID_PRESSURE_MAX,
                   value);
}

const char * pressure_config::key() const
{
    return "liquid_pressure";
}

float pressure_config::load_near_normalize() const
{
    return eps::preferences::instance().get_float(key_near(),
                to_normalize(LIQUID_PRESSURE_NEAR_MIN,
                             LIQUID_PRESSURE_NEAR_MAX,
                             LIQUID_PRESSURE_NEAR_DEFAULT));
}

void pressure_config::save_near_normalize(float value) const
{
    eps::preferences::instance().set_float(key_near(), value);
}

void pressure_config::save_near_normalize_default() const
{
    eps::preferences::instance().set_float(key_near(),
                to_normalize(LIQUID_PRESSURE_NEAR_MIN,
                             LIQUID_PRESSURE_NEAR_MAX,
                             LIQUID_PRESSURE_NEAR_DEFAULT));
}

float pressure_config::prepare_near(float value) const
{
    return to_real(LIQUID_PRESSURE_NEAR_MIN,
                   LIQUID_PRESSURE_NEAR_MAX,
                   value);
}

const char * pressure_config::key_near() const
{
    return "liquid_pressure_near";
}

// -------------------------------
// elasticity

float elasticity_config::load_normalize() const
{
    return eps::preferences::instance().get_float(key(),
                to_normalize(LIQUID_ELASTICITY_MIN,
                             LIQUID_ELASTICITY_MAX,
                             LIQUID_ELASTICITY_DEFAULT));
}

void elasticity_config::save_normalize(float value) const
{
    eps::preferences::instance().set_float(key(), value);
}

void elasticity_config::save_normalize_default() const
{
    eps::preferences::instance().set_float(key(),
                to_normalize(LIQUID_ELASTICITY_MIN,
                             LIQUID_ELASTICITY_MAX,
                             LIQUID_ELASTICITY_DEFAULT));
}

float elasticity_config::prepare(float value) const
{
    return to_real(LIQUID_ELASTICITY_MIN,
                   LIQUID_ELASTICITY_MAX,
                   value);
}

const char * elasticity_config::key() const
{
    return "liquid_elasticity";
}

// ----------------------------
// restore
void restore_default()
{
    density_config dconfig;
    dconfig.save_normalize_default();

    viscosity_config vconfig;
    vconfig.save_sigma_normalize_default();
    vconfig.save_beta_normalize_default();

    pressure_config pconfig;
    pconfig.save_normalize_default();
    pconfig.save_near_normalize_default();

    elasticity_config econfig;
    econfig.save_normalize_default();
}
