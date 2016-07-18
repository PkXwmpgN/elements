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

#include "pass_character.h"
#include <elements/rendering/utils/model_loader.h>
#include <elements/math/trigonometry.h>

namespace eps {
namespace experiment {
namespace character {

bool pass_character::initialize()
{
    world_.create_component_graph<scene::graph>();
    world_.create_component_camera<scene::camera>();
    world_.create_component_light<scene::light>();

    return process_.initialize();
}

void pass_character::process(float)
{
    world_.process();

    if(auto graph = world_.get_component_graph<scene::graph>().lock())
    {
        graph->process(scene::visit_targets<rendering::model>(), process_,
                       world_.get_component_camera<scene::camera>(),
                       world_.get_component_light<scene::light>());
    }
}

bool pass_character::set_model(const std::string & asset)
{
    if(auto graph = world_.get_component_graph<scene::graph>().lock())
    {
        graph->clear();
        return rendering::load_model(asset, graph->add_node<scene::node>());
    }

    return false;
}

utils::unique<rendering::pass_target> pass_character::construct(const math::uvec2 & size)
{
    if(auto camera = world_.get_component_camera<scene::camera>().lock())
    {
        camera->set_aspect(float(size.x) / float(size.y));
        camera->set_eye(math::vec3(0.0f, 0.0f, 5.0f));
    }

    if(auto light = world_.get_component_light<scene::light>().lock())
    {
        light->set_pos(math::vec3(20.0f, 20.0f, 20.0f));
    }

    return nullptr;
}

void pass_character::set_rotation(float theta, float phi)
{
    if(auto camera = world_.get_component_camera<scene::camera>().lock())
    {
        camera->set_eye(5.0f * math::vec3(math::sin(theta) * math::sin(phi),
                                          math::cos(theta),
                                          math::sin(theta) * math::cos(phi)));
    }
}

} /* character */
} /* experiment */
} /* eps */
