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

#include "lpp_lighting_pass.h"
#include "rendering/core/texture_policy.h"
#include "rendering/state/state_macro.h"

namespace eps {
namespace rendering {
namespace techniques {

bool lpp_lighting_pass::initialize()
{
    return process_.initialize();
}

void lpp_lighting_pass::set_scene(const utils::pointer<scene::scene> & scene)
{
    scene_ = scene;
}

void lpp_lighting_pass::process(float)
{
    if(scene_)
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        EPS_STATE_STENCIL_TEST();
        EPS_STATE_DEPTH_READONLY();

        process_.set_map_geometry(get_inputs().get_slot(pass_slot::slot_0));
        process_.set_map_depth(get_inputs().get_slot(pass_slot::slot_1));
        scene_->process_lights(process_, *scene_);
    }
}

} /* techniques */
} /* rendering */
} /* eps */
