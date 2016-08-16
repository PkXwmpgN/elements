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

#ifndef SCENE_ENTITY_MESH_H_INCLUDED
#define SCENE_ENTITY_MESH_H_INCLUDED

#include "utils/std/optional.h"
#include "utils/std/enum.h"
#include <array>

namespace eps {
namespace scene {

struct mesh
{
public:

    enum class feature : short
    {
        geometry,
        material,
        COUNT
    };

    void set_feature(feature id, size_t index)
    {
        feature_[utils::to_int(id)] = index;
    }

    size_t get_feature(feature id) const
    {
        return feature_[utils::to_int(id)];
    }

private:

    std::array
    <
        size_t,
        utils::to_int(feature::COUNT)
    >
    feature_;
};

} /* scene */
} /* eps */

#endif // SCENE_ENTITY_MESH_H_INCLUDED
