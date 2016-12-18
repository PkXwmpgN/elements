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

#ifndef ASSETS_ASSET_SCENE_H_INCLUDED
#define ASSETS_ASSET_SCENE_H_INCLUDED

#include "assets.h"
#include "scene/entity/vertices.h"
#include "scene/graph/node.h"
#include "utils/std/optional.h"

#include <vector>
#include <unordered_map>

struct aiMesh;
struct aiMaterial;
struct aiLight;

namespace eps {

struct asset_scene : public asset
{
public:

    struct geometry
    {
        explicit geometry(const aiMesh * mesh);

        const auto & get_vertices() const { return vertices_; }
        const auto & get_faces() const { return faces_; }

    private:

        void load_vertices(const aiMesh * mesh);
        void load_faces(const aiMesh * mesh);

    private:

        std::vector<scene::vertex> vertices_;
        std::vector<scene::face> faces_;
    };

    using gmap = std::unordered_map<std::string, geometry>;

    struct maps
    {
        maps(const aiMaterial * material, const std::string & path);

        const auto & get_diffuse() const { return map_diffuse_; }
        const auto & get_specular() const { return map_specular_; }
        const auto & get_normal() const { return map_normal_; }

    private:

        void load_maps(const aiMaterial * material,
                       const std::string & path);

    private:

        utils::optional<std::string> map_diffuse_;
        utils::optional<std::string> map_specular_;
        utils::optional<std::string> map_normal_;
    };

    using mmap = std::unordered_map<std::string, maps>;

    struct colors
    {
        explicit colors(const aiMaterial * material);

        const auto & get_diffuse() const { return color_diffuse_; }
        const auto & get_specular() const { return color_specular_; }
        const auto & get_ambient() const { return color_ambient_; }

    private:

        void load_colors(const aiMaterial * material);

    private:

        utils::optional<math::vec3> color_diffuse_;
        utils::optional<math::vec3> color_specular_;
        utils::optional<math::vec3> color_ambient_;
    };

    using cmap = std::unordered_map<std::string, colors>;

    struct light
    {
        explicit light(const aiLight * light);

        float get_attenuation_c() const { return attenuation_c_; }
        float get_attenuation_l() const { return attenuation_l_; }
        float get_attenuation_q() const { return attenuation_q_; }

        const math::vec3 & get_intensity() const { return intensity_; }

    private:

        void load_light(const aiLight * light);

    private:

        float attenuation_c_ = 1.0f;
        float attenuation_l_ = 0.0f;
        float attenuation_q_ = 0.5f;

        math::vec3 intensity_;
    };

    using lmap = std::unordered_map<std::string, light>;

public:

    using asset::asset;
    bool load(utils::link<io::system> fs, const std::string & resource) final;

public:

    utils::pointer<scene::node> get_hierarchy() const;

    const geometry * get_node_geometry(const std::string & name) const;
    const maps * get_node_maps(const std::string & name) const;
    const colors * get_node_colors(const std::string & name) const;
    const light * get_node_light(const std::string & name) const;

private:

    utils::pointer<scene::node> hierarchy_;

    gmap geometry_;
    mmap maps_;
    cmap colors_;
    lmap lights_;
};

} /* eps */

#endif // ASSETS_ASSET_SCENE_H_INCLUDED
