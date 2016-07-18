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

#include "character_renderer.h"
#include "logging.h"

#include <android/input.h>
#include <elements/timing/update.h>
#include <elements/metrics/metrics.h>

character_renderer::character_renderer(bool preview)
    : rate_(60)
    , preview_(preview)
{}

bool character_renderer::startup(const eps::math::uvec2 & size)
{
    glEnable(GL_DEPTH_TEST);

    if(!renderer_)
    {
        eps::utils::unique<eps::experiment::character::renderer> local;
        local.reset(new eps::experiment::character::renderer());
        if(!local->initialize())
        {
            LOGE("[Character] initializing error\n");
            return false;
        }
        renderer_.reset(local.release());
    }

    if(renderer_->construct(size))
    {
        if(preview_)
        {
            ui_.reset(new eps::ui::system());
            ui_->construct(size);

            create_diagnostic_ui(size);
        }
    }

    return true;
}

bool character_renderer::create_diagnostic_ui(const eps::math::uvec2 &)
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

void character_renderer::render()
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

bool character_renderer::set_model(const std::string & model)
{
    if(renderer_)
        return renderer_->set_model(model);
    return false;
}

void character_renderer::set_rotation(float theta, float phi)
{
    if(renderer_)
        renderer_->set_rotation(theta, phi);
}
