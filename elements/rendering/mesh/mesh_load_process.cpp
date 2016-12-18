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

#include "mesh_load_process.h"
#include "mesh_storage.h"
#include "mesh.h"

namespace eps {
namespace rendering {

mesh_load_process::mesh_load_process(utils::pointer<scene::scene> scene)
    : scene_(scene)
    , storage_(utils::make_shared<mesh_storage>())
{}

void mesh_load_process::operator()(scene::node & node, const asset_scene & asset)
{
    if(const auto * geometry = asset.get_node_geometry(node.get_name()))
    {
        auto result = scene_->add_node_entity<mesh>(node.shared_from_this(), storage_).lock();

        mesh_storage::geometry_product product(geometry->get_vertices(),
                                               geometry->get_faces());
        result->set_storage_branch(mesh::branch::geometry,
                                   storage_->add_geometry(std::move(product)));

        if(const auto * maps = asset.get_node_maps(node.get_name()))
        {
            mesh_storage::maps_product product;
            if(const auto & diffuse = maps->get_diffuse())
                product.set_diffuse(storage_->register_map(diffuse.value()));
            if(const auto & specular = maps->get_specular())
                product.set_specular(storage_->register_map(specular.value()));
            if(const auto & normal = maps->get_normal())
                product.set_normal(storage_->register_map(normal.value()));

            result->set_storage_branch(mesh::branch::maps,
                                       storage_->add_maps(std::move(product)));
        }

        if(const auto * colors = asset.get_node_colors(node.get_name()))
        {
            mesh_storage::colors_product product;
            if(const auto & diffuse = colors->get_diffuse())
                product.set_diffuse(diffuse.value());
            if(const auto & specular = colors->get_specular())
                product.set_specular(specular.value());
            if(const auto & ambient = colors->get_ambient())
                product.set_ambient(ambient.value());

            result->set_storage_branch(mesh::branch::colors,
                                       storage_->add_colors(std::move(product)));
        }
    }
}

} /* rendering */
} /* eps */
