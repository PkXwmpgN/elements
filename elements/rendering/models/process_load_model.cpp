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

#include "process_load_model.h"
#include "model_warehouse.h"
#include "model.h"

namespace eps {
namespace rendering {

void process_load_model::operator()(scene::node & node, const asset_model & asset)
{
    const auto & geometry = asset.get_node_geometry(node.get_name());
    const auto & material = asset.get_node_material(node.get_name());

    if(!geometry.empty())
    {
        auto warehouse = utils::make_shared<model_warehouse>();

        std::vector<scene::mesh> meshes(geometry.size());
        for(size_t i = 0, end = meshes.size(); i < end; ++i)
        {
            meshes[i].set_feature(scene::mesh::feature::geometry, i);
            meshes[i].set_feature(scene::mesh::feature::material, i);

            warehouse->add_geometry(geometry[i].get_vertices(), geometry[i].get_faces());
            warehouse->add_material(material[i].get_material());
        }

        scene_->add_node_entity<model>(node.shared_from_this(),
                                       std::move(meshes),
                                       std::move(warehouse));
    }
}

} /* rendering */
} /* eps */
