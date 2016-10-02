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

#include "icosphere.h"
#include "rendering/state/state_macro.h"
#include "scene/entity/vertices.h"
#include "math/geometric.h"

#include <vector>
#include <array>
#include <map>
#include <utility>

namespace eps {
namespace rendering {
namespace primitive {

struct icosphere_generator
{
    using triangle_type = std::array<unsigned short, 3>;
    using edge_type = std::pair<triangle_type::value_type, triangle_type::value_type>;

    using vertices_list = std::vector<math::vec3>;
    using indices_list = std::vector<triangle_type>;

    icosphere_generator()
        : vertices_(make_icosahedron_vertices())
        , indices_(make_icosahedron_indices())
    {}

    void subdivide();

    const auto & get_vertices() const { return vertices_; }
    const auto & get_indices() const { return indices_; }

private:

    vertices_list make_icosahedron_vertices();
    indices_list make_icosahedron_indices();

private:

    vertices_list vertices_;
    indices_list indices_;

    std::map<edge_type, triangle_type::value_type> edges_;
};

void icosphere_generator::subdivide()
{
    indices_list result;
    for(const auto & face : indices_)
    {
        triangle_type subface;
        for(size_t i = 0; i < 3; ++i)
        {
            edge_type edge(face[i], face[(i + 1) % 3]);
            if(edge.second < edge.first)
                std::swap(edge.second, edge.first);

            auto inserted = edges_.insert({edge, vertices_.size()});
            subface[i] = inserted.first->second;

            if(inserted.second)
                vertices_.push_back(math::normalize(vertices_[edge.first] +
                                                    vertices_[edge.second]));
        }

        result.push_back({{face[0], subface[0], subface[2]}});
        result.push_back({{face[1], subface[1], subface[0]}});
        result.push_back({{face[2], subface[2], subface[1]}});
        result.push_back({{subface[0], subface[1], subface[2]}});
    }

    result.swap(indices_);
}

icosphere_generator::vertices_list icosphere_generator::make_icosahedron_vertices()
{
    const float x = 0.52573111f;
    const float z = 0.85065080f;

    return vertices_list(
    {
        {-x, 0.0f, z}, {x, 0.0f, z},  {-x, 0.0f, -z}, {x, 0.0f, -z},
        {0.0f, z, x},  {0.0f, z, -x}, {0.0f, -z, x},  {0.0f, -z, -x},
        {z, x, 0.0f},  {-z, x, 0.0f}, {z, -x, 0.0f},  {-z, -x, 0.0f}
    });
}

icosphere_generator::indices_list icosphere_generator::make_icosahedron_indices()
{
    return indices_list(
    {
        {{0, 6, 1}},  {{0, 11, 4}},  {{1, 4, 0}},  {{1, 8, 4}},  {{1, 10, 8}},
        {{2, 5, 3}}, {{2, 9, 5}}, {{2, 11, 9}},  {{3, 7, 2}},  {{3, 10, 7}},
        {{4, 8, 5}}, {{4, 9, 0}}, {{5, 8, 3}}, {{5, 9, 4}}, {{6, 10, 1}},
        {{6, 11, 7}}, {{7, 10, 6}}, {{7, 11, 2}}, {{8, 10, 3}},  {{9, 11, 0}}
    });
}

icosphere::icosphere(size_t subdivisions)
{
    icosphere_generator generator;
    while(subdivisions--)
        generator.subdivide();

    vertices_.allocate(generator.get_vertices().data(),
                       generator.get_vertices().size(), sizeof(math::vec3));

    indices_.allocate(generator.get_indices().data(),
                      generator.get_indices().size() * 3,
                      sizeof(icosphere_generator::triangle_type::value_type));
}

void icosphere::render(program & program, short a_position)
{
    EPS_STATE_VERTICES(vertices_.get_product());
    EPS_STATE_INDICES(indices_.get_product());

    program.attribute_array(a_position, 0, 3, sizeof(math::vec3));
    program.attribute_array_enable(a_position);

    glDrawElements(GL_TRIANGLES, indices_.get_count(), GL_UNSIGNED_SHORT, 0);
}

} /* primitive */
} /* rendering */
} /* eps */
