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

#ifndef SCENE_NODE_H_INCLUDED
#define SCENE_NODE_H_INCLUDED

#include "math/types.h"
#include "utils/std/pointer.h"
#include <list>

namespace eps {
namespace scene {

class node : public utils::enable_shared_from_this<node>
{
public:

    explicit node(utils::link<node> parent);
    virtual ~node();

    node() = default;
    node(node &&) = default;
    node & operator=(node &&) = default;

    const math::mat4 & get_local_matrix() const { return local_; }
    const math::mat4 & get_world_matrix() const { return world_; }

    void set_local_matrix(const math::mat4 & mat) { local_ = mat; }
    void set_world_matrix(const math::mat4 & mat) { world_ = mat; }

    template<typename _Node, typename ..._Args>
    utils::link<_Node> add_node(_Args&& ...args)
    {
        static_assert(std::is_base_of<node, _Node>::value, "failed");

        auto node = utils::make_shared<_Node>(shared_from_this(), std::forward<_Args>(args)...);
        children_.push_back(node);
        return node;
    }

    utils::link<node> get_parent() const { return parent_; }

    auto begin() const { return children_.begin(); }
    auto end() const { return children_.end(); }

    auto begin() { return children_.begin(); }
    auto end() { return children_.end(); }

private:

    utils::link<node> parent_ ;
    std::list<utils::pointer<node>> children_;

    math::mat4 local_;
    math::mat4 world_;
};

} /* scene */
} /* eps*/

#endif // SCENE_NODE_H_INCLUDED
