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

#ifndef RENDERING_MODELS_MODEL_WAREHOUSE_H_INCLUDED
#define RENDERING_MODELS_MODEL_WAREHOUSE_H_INCLUDED

#include <vector>
#include <array>
#include <unordered_map>

#include "scene/entity/vertices.h"
#include "scene/entity/materials.h"
#include "rendering/core/buffer.h"
#include "rendering/core/texture.h"
#include "utils/std/enum.h"
#include "utils/std/optional.h"

namespace eps {
namespace rendering {

class model_warehouse
{
    struct geometry
    {
    public:

        geometry(const std::vector<scene::vertex> & vertices,
                 const std::vector<scene::face> & faces);

        const product_type & get_vertices() const { return vertices_.get_product(); }
        const product_type & get_indices() const { return indices_.get_product(); }
        size_t get_indices_count() const { return indices_.get_count(); }

    private:

        vertices vertices_;
        indices indices_;
    };

    struct material
    {
    public:

        void set_texture(scene::material::type type, const product_type & texture);
        const utils::optional<product_type> & get_texture(scene::material::type type) const;

    private:

        std::array
        <
            utils::optional<product_type>,
            utils::to_int(scene::material::type::COUNT)
        >
        textures_;
    };

public:

    void add_geometry(const std::vector<scene::vertex> & vertices,
                      const std::vector<scene::face> & faces);
    void add_material(const scene::material & material);

    const auto & get_geometry(size_t index) { return geometry_[index]; }
    const auto & get_material(size_t index) { return materials_[index]; }

private:

    std::vector<geometry> geometry_;
    std::vector<material> materials_;

    std::unordered_map<std::string, texture> textures_;
};

} /* rendering */
} /* eps */

#endif // RENDERING_MODELS_MODEL_WAREHOUSE_H_INCLUDED
