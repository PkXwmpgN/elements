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

#include "light_renderer.h"
#include "logging.h"

#include <android/input.h>
#include <elements/timing/update.h>
#include <elements/math/transform.h>
#include <elements/metrics/metrics.h>

light_renderer::light_renderer(bool preview)
    : rate_(60)
    , preview_(preview)
{}

bool light_renderer::startup(const eps::math::uvec2 & size, size_t particles_count)
{
    if(!simulation_)
        simulation_.reset(new eps::simulation::air::system);

    if(!renderer_)
    {
        eps::utils::unique<eps::experiment::light::renderer> local;
        local.reset(new eps::experiment::light::renderer());
        if(!local->initialize())
        {
            LOGE("initializing error\n");
            return false;
        }
        renderer_.reset(local.release());
    }

    if(simulation_->construct(size))
    {
        renderer_->set_field(simulation_->spawn(1.0f / 60.0f));
    }

    if(renderer_->construct(size, particles_count))
    {
        transform_touch_ = eps::math::translate(0.0f, size.y, 0.0f) *
                           eps::math::scale(1.0f, -1.0f, 1.0f);

        if(preview_)
        {
            ui_.reset(new eps::ui::system());
            ui_->construct(size);

            create_diagnostic_ui(size);
        }
    }

    return true;
}

bool light_renderer::create_diagnostic_ui(const eps::math::uvec2 &)
{
    if(ui_)
    {
        using namespace eps::metric_literals;

        label_fps_ = ui_->add<eps::ui::label>();
        if(auto label = label_fps_.lock())
        {
            label->set_size(128_dp, 18_dp);
            label->set_position(8_dp, 58_dp);
            label->set_font("assets/ui/fonts/OpenSans-Regular.ttf", std::floor(18_dp));
            label->set_color(eps::math::vec4(0.9f, 0.25f, 0.2f, 1.0f));
        }
    }
    return false;
}

void light_renderer::render()
{
    if(renderer_)
    {
        renderer_->render(1.0f / 60.0f);

        if(ui_)
        {
            if(auto label = label_fps_.lock())
            {
                char text[64] = "";
                sprintf(text, "FPS: %.2f", rate_.fps());
                label->set_text(text);
            }

            ui_->draw();
        }
    }

    eps::timing::update_and_sleep(rate_);
}

bool light_renderer::set_background(const char * asset)
{
    return renderer_ ? renderer_->set_background(asset) : false;
}

void light_renderer::set_color(const eps::math::vec3 & color)
{
    if(renderer_)
        renderer_->set_color(color);
}

void light_renderer::touch(float x, float y, int action)
{
    if(simulation_)
    {
        const eps::math::vec4 pos = transform_touch_ * eps::math::vec4(x, y, 1.0f, 1.0f);
        if(action == AMOTION_EVENT_ACTION_MOVE ||
           action == AMOTION_EVENT_ACTION_DOWN)
        {
            simulation_->touch_down(pos.x, pos.y);
        }
        else if(action == AMOTION_EVENT_ACTION_UP)
        {
            simulation_->touch_up(pos.x, pos.y);
        }
    }
}
