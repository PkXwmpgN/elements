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

#ifndef PLATFORM_LIQUID_CONFIG_H_INCLUDED
#define PLATFORM_LIQUID_CONFIG_H_INCLUDED

// -------------------------------
// density

struct density_config
{
    float load_normalize() const;
    void  save_normalize(float value) const;
    void  save_normalize_default() const;

    float prepare(float value) const;

    const char * key() const;
};

// -------------------------------
// viscosity

struct viscosity_config
{
    float load_sigma_normalize() const;
    void  save_sigma_normalize(float value) const;
    void  save_sigma_normalize_default() const;

    float prepare_sigma(float value) const;

    const char * key_sigma() const;

    float load_beta_normalize() const;
    void  save_beta_normalize(float value) const;
    void  save_beta_normalize_default() const;

    float prepare_beta(float value) const;

    const char * key_beta() const;
};

// --------------------------------------------
// pressure

struct pressure_config
{
    float load_normalize() const;
    void  save_normalize(float value) const;
    void  save_normalize_default() const;

    float prepare(float value) const;

    const char * key() const;

    float load_near_normalize() const;
    void  save_near_normalize(float value) const;
    void  save_near_normalize_default() const;

    float prepare_near(float value) const;

    const char * key_near() const;
};

// ---------------------------------------------
// elasticity

struct elasticity_config
{
    float load_normalize() const;
    void  save_normalize(float value) const;
    void  save_normalize_default() const;

    float prepare(float value) const;

    const char * key() const;
};

// restore
void restore_default();

#endif // PLATFORM_LIQUID_CONFIG_H_INCLUDED
