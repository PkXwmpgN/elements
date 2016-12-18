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

#ifndef RENDERING_MESH_H_INCLUDED
#define RENDERING_MESH_H_INCLUDED

#include <array>

#include "scene/entity/entity.h"
#include "utils/std/enum.h"

namespace eps {
namespace rendering {

class mesh_storage;
class mesh : public scene::entity
{
public:

    SNAPE_VISITABLE(mesh);

public:

    enum class branch : short
    {
        geometry,
        maps,
        colors,
        COUNT
    };

    mesh(const utils::link<scene::node> & parent,
         const utils::pointer<mesh_storage> & storage);

    utils::link<mesh_storage> get_storage() const;

    void set_storage_branch(branch id, size_t index)
    {
        storage_branches_[utils::to_int(id)] = index;
    }

    size_t get_storage_branch(branch id) const
    {
        return storage_branches_[utils::to_int(id)];
    }

private:

    std::array
    <
        size_t,
        utils::to_int(branch::COUNT)
    >
    storage_branches_;
    utils::pointer<mesh_storage> storage_;
};

} /* rendering */
} /* eps */

#endif // RENDERING_MESH_H_INCLUDED
