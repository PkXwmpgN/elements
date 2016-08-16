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

#include "model_loader.h"

#include "rendering/models/model.h"
#include "rendering/models/model_warehouse.h"

#include "assets/assets_storage.h"
#include "assets/asset_model.h"

namespace eps {
namespace rendering {

utils::link<scene::node> load_model(const std::string & name,
                                    utils::pointer<scene::scene> scene)
{
    auto asset = assets_storage::instance().read<asset_model>(name);
    if(!asset)
        return utils::link<scene::node>();

    auto warehouse = utils::make_shared<model_warehouse>();

    for(size_t i = 0, end = asset->get_geometry_count(); i < end; ++i)
    {
        auto geometry = asset->get_geometry(i);
        warehouse->add_geometry(geometry.get_vertices(), geometry.get_faces());
    }

    for(size_t i = 0, end = asset->get_geometry_count(); i < end; ++i)
    {
        auto material = asset->get_material(i);
        warehouse->add_material(material.get_material());
    }

    auto node = scene->add_node();
    asset->load_hierarchy(node, [scene, warehouse](const utils::link<scene::node> & node,
                                                   const std::vector<scene::mesh> & meshes)
    {
        scene->add_node_entity<model>(node, meshes, warehouse);
    });

    return node;
}

} /* rendering */
} /* eps */
