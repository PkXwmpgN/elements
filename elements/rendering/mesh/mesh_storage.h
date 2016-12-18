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

#ifndef RENDERING_MESH_STORAGE_H_INCLUDED
#define RENDERING_MESH_STORAGE_H_INCLUDED

#include <string>
#include <vector>
#include <unordered_map>

#include "scene/entity/vertices.h"
#include "rendering/core/buffer.h"
#include "rendering/core/texture.h"
#include "utils/std/enum.h"
#include "utils/std/optional.h"

namespace eps {
namespace rendering {

class mesh_storage
{
public:

    struct geometry_product
    {
    public:

        geometry_product(const std::vector<scene::vertex> & vertices,
                         const std::vector<scene::face> & faces);

        const product_type & get_vertices() const { return vertices_.get_product(); }
        const product_type & get_indices() const { return indices_.get_product(); }

        size_t get_indices_count() const { return indices_.get_count(); }

    private:

        vertices vertices_;
        indices indices_;
    };

    struct maps_product
    {
    public:

        void set_diffuse(const product_type & map);
        void set_specular(const product_type & map);
        void set_normal(const product_type & map);

        const utils::optional<product_type> & get_diffuse() const;
        const utils::optional<product_type> & get_specular() const;
        const utils::optional<product_type> & get_normal() const;

    private:

        utils::optional<product_type> map_diffuse_;
        utils::optional<product_type> map_specular_;
        utils::optional<product_type> map_normal_;
    };

    struct colors_product
    {
    public:

        void set_diffuse(const math::vec3 & color);
        void set_specular(const math::vec3 & color);
        void set_ambient(const math::vec3 & color);

        const utils::optional<math::vec3> & get_diffuse() const;
        const utils::optional<math::vec3> & get_specular() const;
        const utils::optional<math::vec3> & get_ambient() const;

    private:

        utils::optional<math::vec3> color_diffuse_;
        utils::optional<math::vec3> color_specular_;
        utils::optional<math::vec3> color_ambient_;
    };

public:

    size_t add_geometry(geometry_product && geometry);
    size_t add_maps(maps_product && maps);
    size_t add_colors(colors_product && colors);

    const auto & get_geometry(size_t index) const { return geometry_[index]; }
    const auto & get_maps(size_t index) const { return maps_[index]; }
    const auto & get_colors(size_t index) const{ return colors_[index]; }

    const product_type & register_map(const std::string & name);

private:

    std::vector<geometry_product> geometry_;
    std::vector<maps_product> maps_;
    std::vector<colors_product> colors_;

    std::unordered_map<std::string, texture> maps_cache_;
};

} /* rendering */
} /* eps */

#endif // RENDERING_MESH_STORAGE_H_INCLUDED
