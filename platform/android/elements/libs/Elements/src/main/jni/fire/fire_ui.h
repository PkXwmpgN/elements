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

#ifndef PLATFORM_ANDROID_FIRE_UI_H_INCLUDED
#define PLATFORM_ANDROID_FIRE_UI_H_INCLUDED

#include <elements/ui/controls/slider.h>
#include <elements/preferences/preferences.h>
#include <elements/utils/std/pointer.h>
#include <experiments/fire/renderer.h>

#include "fire_config.h"

//------------------------------------------------
// intensity
class fire_intensity_model : public eps::ui::slider_model
                           , public eps::preferences::on_changed_listener
{
public:

    explicit fire_intensity_model(eps::utils::link<eps::experiment::fire::renderer> fire);
    ~fire_intensity_model();

    float get_value() const final;
    void  set_value(float value) final;

    void on_changed(const std::string & key, float value) final;

private:

    eps::utils::link<eps::experiment::fire::renderer> fire_;
    fire_intensity_config config_;
    float value_;
};

//------------------------------------------------
// intensity
class fire_covering_model : public eps::ui::slider_model
                          , public eps::preferences::on_changed_listener
{
public:

    explicit fire_covering_model(eps::utils::link<eps::experiment::fire::renderer> fire);
    ~fire_covering_model();

    float get_value() const final;
    void  set_value(float value) final;

    void on_changed(const std::string & key, float value) final;

private:

    eps::utils::link<eps::experiment::fire::renderer> fire_;
    fire_covering_config config_;
    float value_;
};

#endif //PLATFORM_ANDROID_FIRE_UI_H_INCLUDED
