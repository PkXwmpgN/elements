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

#include "system.h"
#include <thread>

namespace eps {
namespace simulation {
namespace air {

bool system::construct(const math::uvec2 & size)
{
    return volume_.construct(size);
}

void system::touch_down(float x, float y)
{
    touch_prev_.x = touch_.x;
    touch_prev_.y = touch_.y;
    touch_.x = x;
    touch_.y = y;
}

void system::touch_up(float x, float y)
{
    touch_prev_.x = touch_.x = x;
    touch_prev_.y = touch_.y = y;
}

sync::task<math::vec2>::future system::spawn(float dt)
{
    const size_t size = std::distance(volume_.begin(), volume_.end());
    return simulation_task_.start(size, [this](auto begin, auto /*end*/, float dt)
    {
        this->update_velocities(dt);
        for(auto & cell : volume_)
        {
            begin->x = cell.data[volume::cell::U];
            begin->y = cell.data[volume::cell::V];
            ++begin;
        }

    }, dt);
}

void system::update_velocities(float dt)
{
    const float force = 20.0f;
    volume_.velocity(touch_prev_.x, touch_prev_.y,
                    (touch_.x - touch_prev_.x) * force * dt,
                    (touch_.y - touch_prev_.y) * force * dt);
    volume_.velocity_step(dt);
}

} /* air */
} /* simulation */
} /* eps */
