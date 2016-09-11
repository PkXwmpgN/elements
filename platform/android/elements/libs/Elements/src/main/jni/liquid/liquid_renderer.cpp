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

#include "liquid_renderer.h"
#include "liquid_ui.h"
#include "liquid_config.h"
#include "logging.h"

#include <elements/ui/controls/button.h>
#include <elements/ui/controls/panel.h>
#include <elements/ui/controls/slider.h>
#include <elements/timing/update.h>
#include <elements/preferences/preferences.h>
#include <elements/metrics/metrics.h>
#include <elements/math/transform.h>

#include <android/input.h>

struct quality_params
{
    int particles_count;
    int texture_size;
};

const quality_params qualities[3] =
{
    {500, 348},
    {700, 512},
    {900, 748}
};

static eps::ui::touch_action ui_touch_action(int action)
{
    if(action == AMOTION_EVENT_ACTION_MOVE)
        return eps::ui::touch_action::MOVE;
    if(action == AMOTION_EVENT_ACTION_DOWN)
        return eps::ui::touch_action::DOWN;
    return eps::ui::touch_action::UP;
}

liquid_renderer::liquid_renderer(bool preview)
    : rate_(60)
    , preview_(preview)
{}

bool liquid_renderer::startup(const eps::math::uvec2 & size, size_t quality)
{
    if(liquid_ == nullptr)
        liquid_.reset(new eps::simulation::liquid::system);

    if(renderer_ == nullptr)
    {
        eps::utils::unique<eps::experiment::liquid::renderer> local;
        local.reset(new eps::experiment::liquid::renderer());
        if(!local->initialize())
        {
            LOGE("[Liquid] can't initialize renderer \n");
            return false;
        }
        renderer_.reset(local.release());
    }

    const eps::math::vec2 sim_size(10.0f, 10.0f * ((float)size.y / (float)size.x));
    if(liquid_->construct(sim_size, qualities[quality].particles_count))
    {
        renderer_->set_particles(liquid_->spawn(1.0f / 60.0f));

        if(renderer_->construct(size, sim_size, qualities[quality].texture_size))
        {
            if(preview_)
            {
                ui_.reset(new eps::ui::system());
                ui_->construct(size);
                create_settings_ui(size);
                create_diagnostic_ui(size);
            }
        }

        transform_ = eps::math::translate(0.0f, sim_size.y, 0.0f) *
                     eps::math::scale(sim_size.x, -sim_size.y, 1.0f) *
                     eps::math::scale(1.0f / size.x, 1.0f / size.y, 1.0f);

        transform_touch_ = eps::math::translate(0.0f, size.y, 0.0f) *
                           eps::math::scale(1.0f, -1.0f, 1.0f);
    }

    configure();
    return true;
}

void liquid_renderer::configure()
{
    if(liquid_)
    {
        density_config dconfig;
        liquid_->set_density(dconfig.prepare(dconfig.load_normalize()));


        viscosity_config vconfig;
        liquid_->set_viscosity(vconfig.prepare_sigma(vconfig.load_sigma_normalize()),
                              vconfig.prepare_beta(vconfig.load_beta_normalize()));

        pressure_config pconfig;
        liquid_->set_pressure(pconfig.prepare(pconfig.load_normalize()),
                             pconfig.prepare_near(pconfig.load_near_normalize()));

        elasticity_config econfig;
        liquid_->set_elasticity(econfig.prepare(econfig.load_normalize()));
    }
}

void liquid_renderer::render()
{
    if(renderer_)
        renderer_->render();

    if(ui_)
    {
        if(auto label = label_fps_.lock())
        {
            char fps[16] = "";
            sprintf(fps, "FPS: %.2f", rate_.fps());
            label->set_text(fps);
        }
        ui_->draw();
    }

    eps::timing::update_and_sleep(rate_);
}

void liquid_renderer::acceleration(float x, float y, float)
{
    if(liquid_)
        liquid_->set_gravity(eps::math::vec2(x, y));
}


bool liquid_renderer::set_background(const char * asset)
{
    return renderer_ ? renderer_->set_surface_background(std::string(asset)) : false;
}

void liquid_renderer::set_color(float r, float g, float b, float a)
{
    if(renderer_)
        renderer_->set_surface_color(eps::math::vec4(r, g, b, a));
}

void liquid_renderer::touch(float x, float y, int action)
{
    if(renderer_ && liquid_)
    {
        const eps::math::vec4 pos_touch = transform_touch_ * eps::math::vec4(x, y, 1.0f, 1.0f);
        if(!ui_ || !ui_->touch(pos_touch.x, pos_touch.y, ui_touch_action(action), eps::ui::touch_finger::finger0))
        {
            const eps::math::vec4 pos = transform_ * eps::math::vec4(x, y, 1.0f, 1.0f);
            if(action == AMOTION_EVENT_ACTION_MOVE)
                liquid_->touch(eps::math::vec2(pos.x, pos.y), 1.0f);
            else
                liquid_->touch(eps::math::vec2(pos.x, pos.y), 0.0f);
        }
    }
}

bool liquid_renderer::create_diagnostic_ui(const eps::math::uvec2 &)
{
    if(ui_)
    {
        using namespace eps::metric_literals;

        label_fps_ = ui_->add<eps::ui::label>();
        if(auto label = label_fps_.lock())
        {
            label->set_size(128_dp, 18_dp);
            label->set_position(8_dp, 58_dp);
            label->set_font("assets/ui/fonts/OpenSans-Bold.ttf", std::floor(18_dp));
            label->set_color(eps::math::vec4(0.0f, 0.65f, 0.95f, 1.0f));
        }
    }
    return false;
}

bool liquid_renderer::create_settings_ui(const eps::math::uvec2 & size)
{
    // TODO: move this stuff to an ui configuration file

    if(ui_)
    {
        using namespace eps::metric_literals;

        // panel background
        auto panel_link = ui_->add<eps::ui::panel>();
        if(auto panel = panel_link.lock())
        {
            panel->set_size(std::min(size.x - 8_dp, 384_dp), 256_dp);
            panel->set_position(size.x - 4_dp - panel->get_size().x,
                                size.y - panel->get_size().y - 28_dp);
            panel->set_color(eps::math::vec4(0.21f, 0.27f, 0.30f, 0.5f));
            panel->set_visible(false);

            // top panel
            if(auto header = panel->add<eps::ui::panel>().lock())
            {
                header->set_size(panel->get_size().x - 4_dp, 40_dp);
                header->set_position(2_dp, panel->get_size().y - 42_dp);
                header->set_color(eps::math::vec4(0.0f, 0.65f, 0.95f, 0.5f));
            }

            // bottom panel
            if(auto bottom = panel->add<eps::ui::panel>().lock())
            {
                bottom->set_size(panel->get_size().x - 4_dp, 40_dp);
                bottom->set_position(2_dp, 2_dp);
                bottom->set_color(eps::math::vec4(0.0f, 0.65f, 0.95f, 0.5f));
            }

            // title
            if(auto label = panel->add<eps::ui::label>().lock())
            {
                label->set_size(panel->get_size().x - 16_dp, 16_dp);
                label->set_position(8_dp, panel->get_size().y - 30_dp);
                label->set_font("assets/ui/fonts/OpenSans-Bold.ttf", std::floor(16_dp));
                label->set_color(eps::math::vec4(0.8f, 0.8f, 0.8f, 1.0f));
                label->set_text("Physics");
            }

            // viscosity title
            if(auto label = panel->add<eps::ui::label>().lock())
            {
                label->set_size(panel->get_size().x - 32_dp, 16_dp);
                label->set_position(16_dp, panel->get_size().y - 80_dp);
                label->set_font("assets/ui/fonts/OpenSans-Regular.ttf", std::floor(16_dp));
                label->set_color(eps::math::vec4(0.8f, 0.8f, 0.8f, 1.0f));
                label->set_text("Viscosity");
            }

            // viscosity slider
            if(auto slider = panel->add<eps::ui::slider>(new viscosity_model(liquid_)).lock())
            {
                slider->set_size(panel->get_size().x - 128_dp, 16_dp);
                slider->set_position(112_dp, panel->get_size().y - 80_dp);
                slider->set_color_tracker(eps::math::vec4(0.0f, 0.65f, 0.95f, 1.0));
            }

            // density title
            if(auto label = panel->add<eps::ui::label>().lock())
            {
                label->set_size(panel->get_size().x - 32_dp, 16_dp);
                label->set_position(16_dp, panel->get_size().y - 116_dp);
                label->set_font("assets/ui/fonts/OpenSans-Regular.ttf", std::floor(16_dp));
                label->set_color(eps::math::vec4(0.8f, 0.8f, 0.8f, 1.0f));
                label->set_text("Density");
            }

            // density slider
            if(auto slider = panel->add<eps::ui::slider>(new density_model(liquid_)).lock())
            {
                slider->set_size(panel->get_size().x - 128_dp, 16_dp);
                slider->set_position(112_dp, panel->get_size().y - 116_dp);
                slider->set_color_tracker(eps::math::vec4(0.0f, 0.65f, 0.95f, 1.0));
            }

            // pressure title
            if(auto label = panel->add<eps::ui::label>().lock())
            {
                label->set_size(panel->get_size().x - 32_dp, 16_dp);
                label->set_position(16_dp, panel->get_size().y - 150_dp);
                label->set_font("assets/ui/fonts/OpenSans-Regular.ttf", std::floor(16_dp));
                label->set_color(eps::math::vec4(0.8f, 0.8f, 0.8f, 1.0f));
                label->set_text("Pressure");
            }

            // pressure slider
            if(auto slider = panel->add<eps::ui::slider>(new pressure_model(liquid_)).lock())
            {
                slider->set_size(panel->get_size().x - 128_dp, 16_dp);
                slider->set_position(112_dp, panel->get_size().y - 152_dp);
                slider->set_color_tracker(eps::math::vec4(0.0f, 0.65f, 0.95f, 1.0));
            }

            // elastisity title
            if(auto label = panel->add<eps::ui::label>().lock())
            {
                label->set_size(panel->get_size().x - 32_dp, 16_dp);
                label->set_position(16_dp, panel->get_size().y - 188_dp);
                label->set_font("assets/ui/fonts/OpenSans-Regular.ttf", std::floor(16_dp));
                label->set_color(eps::math::vec4(0.8f, 0.8f, 0.8f, 1.0f));
                label->set_text("Elasticity");
            }

            // elastisity slider
            if(auto slider = panel->add<eps::ui::slider>(new elasticity_model(liquid_)).lock())
            {
                slider->set_size(panel->get_size().x - 128_dp, 16_dp);
                slider->set_position(112_dp, panel->get_size().y - 188_dp);
                slider->set_color_tracker(eps::math::vec4(0.0f, 0.65f, 0.95f, 1.0));
            }

            // restore button
            if(auto button = panel->add<eps::ui::button>().lock())
            {
                button->set_asset("assets/ui/textures/restore.png");
                button->set_size(40_dp, 40_dp);
                button->set_position(panel->get_size().x - 48_dp, 2_dp);
                button->set_click([this]()
                {
                    restore_default();
                    configure();
                });
            }

            // settings button
            if(auto button = ui_->add<eps::ui::button>().lock())
            {
                button->set_asset("assets/ui/textures/settings.png");
                button->set_size(40_dp, 40_dp);
                button->set_position(size.x - 50_dp, size.y - 70_dp);
                button->set_click([link = std::move(panel_link)]()
                {
                    if(auto panel = link.lock())
                        panel->set_visible(!panel->get_visible());
                });
            }
        }

        return true;
    }

    return false;
}
