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
#include "math/transform.h"
#include "utils/std/pointer.h"
#include <list>

namespace eps {
namespace scene {

class node : public utils::enable_shared_from_this<node>
{
public:

    enum class state : uint8_t
    {
        live,
        dead
    };

public:

    node() = default;
    explicit node(utils::link<node> parent);

    const math::mat4 & get_local_matrix() const { return local_; }
    const math::mat4 & get_world_matrix() const { return world_; }

    void set_local_matrix(const math::mat4 & local) { local_ = local; }
    void set_world_matrix(const math::mat4 & world) { world_ = world; }

    utils::link<node> add_node();
    utils::link<node> get_parent() const { return parent_; }

    template<typename _Operation>
    void process(_Operation && ops);

    void set_state(const state & s) { state_ = s; }
    const state & get_state() const { return state_; }

    bool clear();

private:

    utils::link<node> parent_;
    std::list<utils::pointer<node>> children_;

    math::mat4 local_;
    math::mat4 world_;

    state state_;
};

math::vec3 get_position(const node & sn);
math::vec3 get_position(const utils::link<node> & sn);

void set_position(node & sn, const math::vec3 & pos);
void set_position(const utils::link<node> & sn, const math::vec3 & pos);

template<typename _Operation>
void node::process(_Operation && opt)
{
    opt(*this);
    for(auto & child : children_)
        child->process(opt);
}

inline math::vec3 get_position(const node & sn)
{
    return math::vec3(sn.get_world_matrix()[3]);
}

inline math::vec3 get_position(const utils::link<node> & link)
{
    if(auto sn = link.lock())
        return get_position(*sn);
    return math::vec3();
}

inline void set_position(node & sn, const math::vec3 & pos)
{
    sn.set_local_matrix(math::translate(pos));
}

inline void set_position(const utils::link<node> & link, const math::vec3 & pos)
{
    if(auto sn = link.lock())
        set_position(*sn, pos);
}

} /* scene */
} /* eps*/

#endif // SCENE_NODE_H_INCLUDED
