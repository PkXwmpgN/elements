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

#include "scene.h"

namespace eps {
namespace scene {

scene::scene()
    : root_(utils::make_shared<node>("Root"))
{}

utils::link<camera> scene::get_camera() const
{
    // TODO: process to changing an active camera
    // default the first

    if(cameras_.size())
        return std::begin(cameras_)->second;

    return utils::link<camera>();
}

utils::link<camera> scene::get_camera(const std::string & name) const
{
    auto it = cameras_.find(name);
    return it != std::end(cameras_) ? it->second : utils::link<camera>();
}

void scene::update(float dt)
{
    // 1. clean dead nodes
    if(root_->clear())
    {
        static auto dead = [](auto & entry){ return entry->get_node().expired(); };

        modifiers_.remove_if(dead);
        entities_.remove_if(dead);

        for(auto it = begin(cameras_); it != end(cameras_);)
        {
            if(dead(it->second))
                it = cameras_.erase(it);
            else
                ++it;
        }

        for(auto it = begin(lights_); it != end(lights_);)
        {
            if(dead(it->second))
                it = lights_.erase(it);
            else
                ++it;
        }
    }

    // 2. process local matrices
    for(auto & sm : modifiers_)
        sm->process(dt);

    // 3, process projection and view matrices
    if(auto camera = get_camera().lock())
        camera->process(dt);

    // 4. process world matrices
    static auto process_world_matrices = [](auto & node)
    {
        if(auto parent = node.get_parent().lock())
            node.set_world_matrix(parent->get_world_matrix() * node.get_local_matrix());
        else
            node.set_world_matrix(node.get_local_matrix());
    };
    root_->process(process_world_matrices);
}

} /* scene */
} /* eps */
