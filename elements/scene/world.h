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

#ifndef SCENE_WORLD_H_INCLUDED
#define SCENE_WORLD_H_INCLUDED

#include "utils/std/pointer.h"
#include "camera/camera.h"
#include "light/light.h"
#include "graph/graph.h"

#include <type_traits>

namespace eps {
namespace scene {

class world
{
public:

    void process();

    template<typename _Graph, typename ..._Args>
    void create_component_graph(_Args&& ...args)
    {
        static_assert(std::is_base_of<graph, _Graph>::value, "failed");
        component_graph_ = utils::make_shared<_Graph>(std::forward<_Args>(args)...);
    }

    template<typename _Camera, typename ..._Args>
    void create_component_camera(_Args&& ...args)
    {
        static_assert(std::is_base_of<camera, _Camera>::value, "failed");
        component_camera_ = utils::make_shared<_Camera>(std::forward<_Args>(args)...);
    }

    template<typename _Light, typename ..._Args>
    void create_component_light(_Args&& ...args)
    {
        static_assert(std::is_base_of<light, _Light>::value, "failed");
        component_light_ = utils::make_shared<_Light>(std::forward<_Args>(args)...);
    }

    template<typename _Graph>
    utils::link<_Graph> get_component_graph() const { return component_graph_; }

    template<typename _Camera>
    utils::link<_Camera> get_component_camera() const { return component_camera_; }

    template<typename _Light>
    utils::link<_Light> get_component_light() const { return component_light_; }

private:

    utils::pointer<graph> component_graph_;
    utils::pointer<camera> component_camera_;
    utils::pointer<light> component_light_;
};

} /* scene */
} /* eps */

#endif // SCENE_WORLD_H_INCLUDED
