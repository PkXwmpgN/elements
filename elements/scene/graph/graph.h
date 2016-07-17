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

#ifndef SCENE_GRAPH_H_INCLUDED
#define SCENE_GRAPH_H_INCLUDED

#include "math/types.h"
#include "utils/std/pointer.h"
#include "visitor.h"
#include "node.h"

namespace eps {
namespace scene {

class graph
{
public:

    graph();
    graph(graph&&) = default;
    graph & operator=(graph&&) = default;
    virtual ~graph() {}

    void process(const math::mat4 & view);

    template <typename... _Targets, typename _Operation, typename... _Args>
    void process(visit_targets<_Targets...> targets, _Operation && ops, _Args&& ...args)
    {
        scene::visit(targets, root_, std::forward<_Operation>(ops),
                     std::forward<_Args>(args)...);
    }

    template<typename _Node, typename ..._Args>
    auto add_node(_Args&& ...args)
    {
        return root_->add_node<_Node>(std::forward<_Args>(args)...);
    }

    void clear();

private:

    utils::pointer<node> root_;
};

} /* scene */
} /* eps */

#endif // SCENE_GRAPH_H_INCLUDED
