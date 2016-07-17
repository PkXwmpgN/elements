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

#ifndef SCENE_SCENE_VISITOR_H_INCLUDED
#define SCENE_SCENE_VISITOR_H_INCLUDED

#include "utils/std/pointer.h"

namespace eps {
namespace scene {

template <typename... _TArgs>
class visit_targets {};

namespace details {

    template<typename _To, typename _From>
    inline utils::pointer<_To> cast_node(const utils::pointer<_From> & node)
    {
        return utils::dynamic_pointer_cast<_To>(node);
    }

    template<typename _Node, typename _Operation, typename... _Args>
    inline void call(_Node && node, _Operation && ops, _Args&& ...args)
    {
        if(node)
            ops(std::forward<_Node>(node), std::forward<_Args>(args)...);
    }

    template<typename _Node, typename _Operation, typename... _Args>
    inline void call(visit_targets<>, _Node &&, _Operation &&, _Args&& ...) {}

    template<typename _Target, typename... _Targets, typename _Node,
             typename _Operation, typename... _Args>
    inline void call(visit_targets<_Target, _Targets...>, _Node && node, _Operation && ops,
                     _Args&& ...args)
    {
        call(cast_node<_Target>(std::forward<_Node>(node)),
             std::forward<_Operation>(ops),
             std::forward<_Args>(args)...);

        call(visit_targets<_Targets...>(),
             std::forward<_Node>(node),
             std::forward<_Operation>(ops),
             std::forward<_Args>(args)...);
    }
}

template <typename... _Targets, typename _Node, typename _Operation, typename... _Args>
inline void visit(visit_targets<_Targets...> targets, _Node && node, _Operation && ops,
                  _Args&& ...args)
{
    static_assert(sizeof...(_Targets) > 0, "failed");

    details::call(targets,
                  std::forward<_Node>(node),
                  std::forward<_Operation>(ops),
                  std::forward<_Args>(args)...);

    for(auto & child : *node)
        visit(targets, child, std::forward<_Operation>(ops), std::forward<_Args>(args)...);
}

} /* scene */
} /* eps */

#endif // SCENE_SCENE_VISITOR_H_INCLUDED
