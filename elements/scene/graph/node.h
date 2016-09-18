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
#include <string>

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

    explicit node(const std::string & name);

    const math::mat4 & get_local_matrix() const { return local_; }
    const math::mat4 & get_world_matrix() const { return world_; }

    void set_local_matrix(const math::mat4 & local) { local_ = local; }
    void set_world_matrix(const math::mat4 & world) { world_ = world; }

    utils::link<node> attach_node(utils::pointer<node> child);
    utils::link<node> add_node(const std::string & name);

    utils::link<node> get_parent() const { return parent_; }
    void set_parent(utils::link<node> parent) { parent_ = parent; }

    const std::string & get_name() const { return name_; }

    template<typename _Operation, typename... _Args>
    void process(_Operation && ops, _Args&& ...args);

    void set_state(const state & s) { state_ = s; }
    const state & get_state() const { return state_; }

    bool clear();

private:

    std::string name_;

    utils::link<node> parent_;
    std::list<utils::pointer<node>> children_;

    math::mat4 local_;
    math::mat4 world_;

    state state_;
};


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

inline utils::pointer<node> make_root(const std::string & name)
{
    return utils::make_shared<node>(name);
}

template<typename _Operation, typename... _Args>
void node::process(_Operation && opt, _Args&& ...args)
{
    opt(*this, std::forward<_Args>(args)...);
    for(auto & child : children_)
        child->process(opt, std::forward<_Args>(args)...);
}

} /* scene */
} /* eps*/

#endif // SCENE_NODE_H_INCLUDED
