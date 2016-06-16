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

#include "fire_config.h"

#include <elements/preferences/preferences.h>

const float FIRE_INTENSITY_MIN = 0.0f;
const float FIRE_INTENSITY_MAX = 1.0f;
const float FIRE_INTENSITY_DEFAULT = 0.5f;

const float FIRE_COVERING_MIN = 0.0f;
const float FIRE_COVERING_MAX = 1.0f;
const float FIRE_COVERING_DEFAULT = 1.0f;


static inline float to_real(float min, float max, float value)
{
    return (max - min) * value + min;
}

static inline float to_normalize(float min, float max, float value)
{
    return (value - min) / (max - min);
}

// -------------------------------
// intensity

float fire_intensity_config::load_normalize() const
{
    return eps::preferences::instance().get_float(key(),
                to_normalize(FIRE_INTENSITY_MIN,
                             FIRE_INTENSITY_MAX,
                             FIRE_INTENSITY_DEFAULT));
}

void fire_intensity_config::save_normalize(float value) const
{
    eps::preferences::instance().set_float(key(), value);
}

void fire_intensity_config::save_normalize_default() const
{
    eps::preferences::instance().set_float(key(),
                to_normalize(FIRE_INTENSITY_MIN,
                             FIRE_INTENSITY_MAX,
                             FIRE_INTENSITY_DEFAULT));
}

float fire_intensity_config::prepare(float value) const
{
    return to_real(FIRE_INTENSITY_MIN,
                   FIRE_INTENSITY_MAX,
                   value);
}

const char * fire_intensity_config::key() const
{
    return "fire_intensity";
}

// -------------------------------
// covering

float fire_covering_config::load_normalize() const
{
    return eps::preferences::instance().get_float(key(),
                to_normalize(FIRE_COVERING_MIN,
                             FIRE_COVERING_MAX,
                             FIRE_COVERING_DEFAULT));
}

void fire_covering_config::save_normalize(float value) const
{
    eps::preferences::instance().set_float(key(), value);
}

void fire_covering_config::save_normalize_default() const
{
    eps::preferences::instance().set_float(key(),
                to_normalize(FIRE_COVERING_MIN,
                             FIRE_COVERING_MAX,
                             FIRE_COVERING_DEFAULT));
}

float fire_covering_config::prepare(float value) const
{
    return to_real(FIRE_COVERING_MIN,
                   FIRE_COVERING_MAX,
                   value);
}

const char * fire_covering_config::key() const
{
    return "fire_covering";
}

// ----------------------------------
// defaults

void fire_restore_default()
{
    fire_intensity_config iconfig;
    iconfig.save_normalize_default();

    fire_covering_config cconfig;
    cconfig.save_normalize_default();
}
