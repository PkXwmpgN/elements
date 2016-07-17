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

#ifndef ASSETS_ASSET_MODEL_H_INCLUDED
#define ASSETS_ASSET_MODEL_H_INCLUDED

#include "assets.h"
#include "scene/object/vertices.h"
#include "scene/object/materials.h"
#include "scene/object/object.h"
#include "scene/graph/node.h"
#include "utils/std/optional.h"
#include "utils/std/pointer.h"
#include <vector>
#include <functional>

struct aiScene;
struct aiMesh;
struct aiMaterial;
struct aiNode;

namespace eps {

struct asset_model : public asset
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

    struct material
    {
        material(const aiMaterial * material, const std::string & path);
        const auto & get_material() const { return material_; }

    private:

        void load_material(const aiMaterial * material,
                           const std::string & path);

    private:

        scene::material material_;
    };

    using node_inserter = std::function<utils::link<scene::object>(utils::link<scene::node>)>;

public:

    using asset::asset;
    bool load(utils::link<io::system> fs, const std::string & resource) final;

    size_t get_geometry_count() const;
    size_t get_material_count() const;

    geometry get_geometry(size_t index) const;
    material get_material(size_t index) const;

    void load_hierarchy(utils::link<scene::node> node, node_inserter inserter) const;

private:

    utils::pointer<aiScene> scene_;
    std::string path_;
};

} /* eps */

#endif // ASSETS_ASSET_MODEL_H_INCLUDED
