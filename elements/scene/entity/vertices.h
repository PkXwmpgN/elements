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

#ifndef SCENE_ENTITY_VERTICES_H_INCLUDED
#define SCENE_ENTITY_VERTICES_H_INCLUDED

#include "math/types.h"

namespace eps {
namespace scene {

struct vertex
{
    math::vec3 position;
    math::vec3 normal;
    math::vec3 tangent;
    math::vec2 tex;
};

static_assert(offsetof(vertex, position) == 0, "failed");
static_assert(offsetof(vertex, normal) == sizeof(math::vec3), "failed");
static_assert(offsetof(vertex, tangent) == sizeof(math::vec3) * 2, "failed");
static_assert(offsetof(vertex, tex) == sizeof(math::vec3) * 3, "failed");

struct face
{
    static const short FACE_INDICES_COUNT = 3;
    unsigned short indices[FACE_INDICES_COUNT];
};

static_assert(face::FACE_INDICES_COUNT == 3, "failed");

} /* scene */
} /* eps */

#endif // SCENE_ENTITY_VERTICES_H_INCLUDED
