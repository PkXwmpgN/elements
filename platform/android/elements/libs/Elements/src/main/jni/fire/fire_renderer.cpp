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

#include "fire_renderer.h"
#include "fire_ui.h"
#include "fire_config.h"
#include "logging.h"
#include <chrono>

#include <android/input.h>
#include <elements/ui/controls/button.h>
#include <elements/ui/controls/panel.h>
#include <elements/ui/controls/slider.h>
#include <elements/timing/update.h>
#include <elements/math/transform.h>
#include <elements/metrics/metrics.h>

const size_t texture_size[] = { 256, 384, 512 };

static eps::ui::touch_action ui_touch_action(int action)
{
    if(action == AMOTION_EVENT_ACTION_MOVE)
        return eps::ui::touch_action::MOVE;
    if(action == AMOTION_EVENT_ACTION_DOWN)
        return eps::ui::touch_action::DOWN;
    return eps::ui::touch_action::UP;
}

fire_renderer::fire_renderer(bool preview)
    : rate_(60)
    , preview_(preview)
{}

bool fire_renderer::startup(const eps::math::uvec2 & size, size_t quality)
{
    if(!renderer_)
    {
        eps::utils::pointer<eps::experiment::fire::renderer> local;
        local.reset(new eps::experiment::fire::renderer());
        if(!local->initialize())
        {
            LOGE("Can't initialize renderer\n");
            return false;
        }
        renderer_ = local;
    }

    if(renderer_->construct(size, texture_size[quality]))
    {
        transform_touch_ = eps::math::translate(0.0f, size.y, 0.0f) *
                           eps::math::scale(1.0f, -1.0f, 1.0f);

        if(preview_)
        {
            ui_.reset(new eps::ui::system());
            ui_->construct(size);
            create_settings_ui(size);
            create_diagnostic_ui(size);
        }

        timer_.start();
    }

    configure();

    return true;
}

void fire_renderer::configure()
{
    fire_intensity_config iconfig;
    set_intensity(iconfig.prepare(iconfig.load_normalize()));

    fire_covering_config cconfig;
    set_covering(cconfig.prepare(cconfig.load_normalize()));
}

void fire_renderer::render()
{
    if(renderer_)
    {
        renderer_->render(timer_.update());

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
    }

    eps::timing::update_and_sleep(rate_);
}

bool fire_renderer::set_background(const char * asset)
{
    return renderer_ ? renderer_->set_background(asset) : false;
}

void fire_renderer::set_intensity(float value)
{
    if(renderer_)
        renderer_->set_intensity(value);
}

void fire_renderer::set_covering(float value)
{
    if(renderer_)
        renderer_->set_covering(value);
}

void fire_renderer::set_color_hot(const eps::math::vec3 & color)
{
    if(renderer_)
        renderer_->set_color_hot(color);
}

void fire_renderer::set_color_cold(const eps::math::vec3 & color)
{
    if(renderer_)
        renderer_->set_color_cold(color);
}

void fire_renderer::touch(float x, float y, int action)
{
    if(ui_ && renderer_)
    {
        const eps::math::vec4 pos = transform_touch_ * eps::math::vec4(x, y, 1.0f, 1.0f);
        ui_->touch(pos.x, pos.y, ui_touch_action(action));
    }
}


bool fire_renderer::create_settings_ui(const eps::math::uvec2 & size)
{
    // TODO: move this stuff to an ui configuration file

    if(ui_)
    {
        using namespace eps::metric_literals;

        // -----------------------------------
        // panel
        auto panel_link = ui_->add<eps::ui::panel>();
        if(auto panel = panel_link.lock())
        {
            panel->set_size(std::min(size.x - 8_dp, 384_dp), 186_dp);
            panel->set_position(size.x - 4_dp - panel->get_size().x, (size.y - panel->get_size().y - 28_dp));
            panel->set_color(eps::math::vec4(0.21f, 0.27f, 0.30f, 0.5f));
            panel->set_visible(false);

            // top panel
            if(auto header = panel->add<eps::ui::panel>().lock())
            {
                header->set_size(panel->get_size().x - 4_dp, 40_dp);
                header->set_position(2_dp, panel->get_size().y - 42_dp);
                header->set_color(eps::math::vec4(0.9f, 0.25f, 0.2f, 0.5f));
            }

            // botoom panel
            if(auto bottom = panel->add<eps::ui::panel>().lock())
            {
                bottom->set_size(panel->get_size().x - 4_dp, 40_dp);
                bottom->set_position(2_dp, 2_dp);
                bottom->set_color(eps::math::vec4(0.9f, 0.25f, 0.2f, 0.5f));
            }

            // title
            if(auto label = panel->add<eps::ui::label>().lock())
            {
                label->set_size(panel->get_size().x - 16_dp, 16_dp);
                label->set_position(8_dp, panel->get_size().y - 26_dp);
                label->set_font("assets/ui/fonts/OpenSans-Bold.ttf", std::floor(16_dp));
                label->set_color(eps::math::vec4(0.8f, 0.8f, 0.8f, 1.0f));
                label->set_text("Parameters");
            }

            // intensity title
            if(auto label = panel->add<eps::ui::label>().lock())
            {
                label->set_size(panel->get_size().x - 32_dp, 16_dp);
                label->set_position(16_dp, panel->get_size().y - 80_dp);
                label->set_font("assets/ui/fonts/OpenSans-Regular.ttf", std::floor(16_dp));
                label->set_color(eps::math::vec4(0.8f, 0.8f, 0.8f, 1.0f));
                label->set_text("Intensity");
            }

            // intensity slider
            if(auto slider = panel->add<eps::ui::slider>(new fire_intensity_model(renderer_)).lock())
            {
                slider->set_size(panel->get_size().x - 128_dp, 16_dp);
                slider->set_position(112_dp, panel->get_size().y - 80_dp);
                slider->set_color_tracker(eps::math::vec4(0.9f, 0.25f, 0.2f, 1.0));
            }

            // covering title
            if(auto label = panel->add<eps::ui::label>().lock())
            {
                label->set_size(panel->get_size().x - 32_dp, 16_dp);
                label->set_position(16_dp, panel->get_size().y - 116_dp);
                label->set_font("assets/ui/fonts/OpenSans-Regular.ttf", std::floor(16_dp));
                label->set_color(eps::math::vec4(0.8f, 0.8f, 0.8f, 1.0f));
                label->set_text("Covering");
            }

            // covering slider
            if(auto slider = panel->add<eps::ui::slider>(new fire_covering_model(renderer_)).lock())
            {
                slider->set_size(panel->get_size().x - 128_dp, 16_dp);
                slider->set_position(112_dp, panel->get_size().y - 116_dp);
                slider->set_color_tracker(eps::math::vec4(0.9f, 0.25f, 0.2f, 1.0));
            }

            // restore button
            if(auto button = panel->add<eps::ui::button>().lock())
            {
                button->set_asset("assets/ui/textures/restore.png");
                button->set_size(40_dp, 40_dp);
                button->set_position(panel->get_size().x - 48_dp, 2_dp);
                button->set_click([this]()
                {
                    fire_restore_default();
                    configure();
                });
            }

            // settings button
            if(auto button = ui_->add<eps::ui::button>().lock())
            {
                button->set_asset("assets/ui/textures/settings.png");
                button->set_size(40_dp, 40_dp);
                button->set_position(size.x - 48_dp, size.y - 70_dp);
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

bool fire_renderer::create_diagnostic_ui(const eps::math::uvec2 &)
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
            label->set_color(eps::math::vec4(0.9f, 0.25f, 0.2f, 1.0f));
        }
    }
    return false;
}
