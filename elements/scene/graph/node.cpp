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

#include "node.h"

namespace eps {
namespace scene {

node::node(const std::string & name)
    : name_(name)
    , local_(1.0f)
    , world_(1.0f)
    , state_(state::live)
{}

bool node::clear()
{
    static auto dead = [](auto & node){ return node->get_state() == state::dead; };

    size_t size = children_.size();
    children_.remove_if(dead);

    bool result = size != children_.size();
    for(auto & child : children_)
        result |= child->clear();

    return result;
}

utils::link<node> node::add_node(const std::string & name)
{
    return attach_node(utils::make_shared<node>(name));
}

utils::link<node> node::attach_node(utils::pointer<node> child)
{
    child->set_parent(shared_from_this());
    children_.push_back(child);
    return children_.back();
}

} /* scene */
} /* eps */
