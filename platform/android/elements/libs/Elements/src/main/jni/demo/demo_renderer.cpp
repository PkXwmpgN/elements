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

#include "demo_renderer.h"
#include "logging.h"

#include <elements/utils/std/enum.h>
#include <elements/timing/update.h>
#include <elements/math/transform.h>
#include <elements/ui/types.h>

#include <android/input.h>

static eps::ui::touch_action ui_touch_action(int action)
{
    if(action == AMOTION_EVENT_ACTION_MOVE)
        return eps::ui::touch_action::MOVE;
    if(action == AMOTION_EVENT_ACTION_UP ||
       action == AMOTION_EVENT_ACTION_POINTER_UP)
        return eps::ui::touch_action::UP;
    return eps::ui::touch_action::DOWN;
}

demo_renderer::demo_renderer()
    : rate_(60)
{}

bool demo_renderer::startup(const eps::math::uvec2 & size)
{
    if(renderer_ == nullptr)
    {
        eps::utils::unique<eps::experiment::demo::renderer> local;
        local.reset(new eps::experiment::demo::renderer());
        if(!local->initialize())
        {
            LOGE("[Demo] can't initialize renderer \n");
            return false;
        }
        renderer_.reset(local.release());
    }

    if(renderer_->construct(size))
    {
        transform_touch_ = eps::math::translate(0.0f, size.y, 0.0f) *
                           eps::math::scale(1.0f, -1.0f, 1.0f);
    }


    return true;
}

void demo_renderer::render()
{
    if(renderer_)
    {
        renderer_->set_rate(rate_.fps());
        renderer_->render(1.0f / 60.0f);
    }

    eps::timing::update_and_sleep(rate_);
}

void demo_renderer::touch(float x, float y, int action, int finger)
{
    if(renderer_ && finger < eps::utils::to_int(eps::ui::touch_finger::COUNT))
    {
        const eps::math::vec4 pos_touch = transform_touch_ * eps::math::vec4(x, y, 1.0f, 1.0f);
        renderer_->touch(eps::math::vec2(pos_touch.x, pos_touch.y), ui_touch_action(action),
                     eps::ui::touch_finger(finger));
    }
}

void demo_renderer::set_rotation(float theta, float phi)
{
    if(renderer_)
        renderer_->set_rotation(theta, phi);
}
