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

#include "camera_load_process.h"
#include "camera.h"
#include "math/common.h"

namespace eps {
namespace scene {

camera_load_process::camera_load_process(utils::pointer<scene> scene)
    : scene_(scene)
{}

void camera_load_process::operator()(node & n, const asset_scene & asset)
{
    if(const auto * light = asset.get_node_light(n.get_name()))
    {
        if(auto pl = scene_->add_node_light<light_point>(n.shared_from_this()).lock())
        {
            const float luminance_min = 0.05f;
            const float luminance_max = 1.0f;

            const float a = light->get_attenuation_q();
            const float b = light->get_attenuation_l();
            const float c = light->get_attenuation_c() - luminance_max / luminance_min;

            float range = 1.0f;
            if(math::equal(a, 0.0f))
            {
                if(!math::equal(b, 0.0f))
                    range = -c / b;
            }
            else
            {
                const float d = b * b - 4 * a * c;
                range = (d >= 0.0f) ? (-b + math::sqrt(d)) / (2.0f * a) : -b / (2.0f * a);
            }

            pl->set_intensity(light->get_intensity());
            pl->set_range(range);
        }
    }
}

} /* rendering */
} /* eps */
