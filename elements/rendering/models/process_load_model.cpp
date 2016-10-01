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
#include "math/common.h"

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

    if(auto light = asset.get_node_light(node.get_name()))
    {
        if(auto pl = scene_->add_node_light<scene::light_point>(node.shared_from_this()).lock())
        {
            const float luminance_min = 0.05f;
            const float luminance_max = 1.0f;

            const float a = light->get_attenuation_q();
            const float b = light->get_attenuation_l();
            const float c = light->get_attenuation_c() - luminance_max / luminance_min;

            float range = 1.0f;
            if(math::equal(a, 0.0f))
            {
                if(!math::equal(b, 0.0f))
                    range = -c / b;
            }
            else
            {
                const float d = b * b - 4 * a * c;
                range = (d >= 0.0f) ? (-b + math::sqrt(d)) / (2.0f * a) : -b / (2.0f * a);
            }

            pl->set_intensity(light->get_intensity());
            pl->set_range(range);
        }
    }
}

} /* rendering */
} /* eps */
