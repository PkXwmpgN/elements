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

#include "process.h"
#include "model_warehouse.h"
#include "rendering/state/state_macro.h"
#include "rendering/utils/program_loader.h"
#include "math/matrix.h"

namespace eps {
namespace rendering {

enum class program_enum : short
{
    // attributes
    a_vertex_pos = 0,
    a_vertex_normal = 1,
    a_vertex_tangent = 2,
    a_vertex_uv = 3,
    // uniforms
    u_matrix_mvp = 0,
    u_matrix_world = 1,
    u_matrix_normal = 2,
    u_camera_pos = 3,
    u_map_diffuse = 4,
    u_map_specular = 5,
    u_map_normal = 6,
    u_light_pos = 7,
    u_light_diffuse = 8,
    u_light_specular = 9,
    u_light_ambient = 10
};

bool process_model_rendering::initialize()
{
    return load_program("assets/shaders/models/model.prog", program_);
}

void process_model_rendering::operator()(const utils::pointer<model> & node,
                                         const utils::link<scene::camera> & link_camera,
                                         const utils::link<scene::light> & link_light)
{
    auto camera = link_camera.lock();
    assert(camera);

    auto light = link_light.lock();
    assert(light);

    auto warehouse = node->get_warehouse().lock();
    assert(warehouse);

    EPS_STATE_PROGRAM(program_.get_product());

    for(const auto & mesh : *node)
    {
        const auto & geometry = warehouse->get_geometry(mesh.get_feature(scene::mesh::feature::geometry));
        const auto & material = warehouse->get_material(mesh.get_feature(scene::mesh::feature::material));

        EPS_STATE_VERTICES(geometry.get_vertices());

        program_.attribute_array_enable(utils::to_int(program_enum::a_vertex_pos));
        program_.attribute_array(utils::to_int(program_enum::a_vertex_pos),
                                 offsetof(scene::vertex, position), 3, sizeof(scene::vertex));

        program_.attribute_array_enable(utils::to_int(program_enum::a_vertex_normal));
        program_.attribute_array(utils::to_int(program_enum::a_vertex_normal),
                                 offsetof(scene::vertex, normal), 3, sizeof(scene::vertex));

        program_.attribute_array_enable(utils::to_int(program_enum::a_vertex_tangent));
        program_.attribute_array(utils::to_int(program_enum::a_vertex_tangent),
                                 offsetof(scene::vertex, tangent), 3, sizeof(scene::vertex));

        program_.attribute_array_enable(utils::to_int(program_enum::a_vertex_uv));
        program_.attribute_array(utils::to_int(program_enum::a_vertex_uv),
                                 offsetof(scene::vertex, tex), 2, sizeof(scene::vertex));

        if(const auto & texture = material.get_texture(scene::material::type::diffuse))
        {
            EPS_STATE_SAMPLER_0(texture.value());
            program_.uniform_value(utils::to_int(program_enum::u_map_diffuse), 0);
        }

        if(const auto & texture = material.get_texture(scene::material::type::specular))
        {
            EPS_STATE_SAMPLER_1(texture.value());
            program_.uniform_value(utils::to_int(program_enum::u_map_specular), 1);
        }

        if(const auto & texture = material.get_texture(scene::material::type::normals))
        {
            EPS_STATE_SAMPLER_2(texture.value());
            program_.uniform_value(utils::to_int(program_enum::u_map_normal), 2);
        }

        program_.uniform_value(utils::to_int(program_enum::u_camera_pos), camera->get_eye());

        program_.uniform_value(utils::to_int(program_enum::u_light_pos), light->get_pos());
        program_.uniform_value(utils::to_int(program_enum::u_light_diffuse), light->get_diffuse());
        program_.uniform_value(utils::to_int(program_enum::u_light_specular), light->get_specular());
        program_.uniform_value(utils::to_int(program_enum::u_light_ambient), light->get_ambient());

        const math::mat4 & world = node->get_world_matrix();
        const math::mat4 & projection = camera->get_projection();
        program_.uniform_value(utils::to_int(program_enum::u_matrix_mvp), projection * world);
        program_.uniform_value(utils::to_int(program_enum::u_matrix_world), world);
        program_.uniform_value(utils::to_int(program_enum::u_matrix_normal),
                               math::transpose(math::inverse(math::mat3(world))));

        EPS_STATE_INDICES(geometry.get_indices());
        glDrawElements(GL_TRIANGLES, geometry.get_indices_count(), GL_UNSIGNED_SHORT, 0);
    }
}

} /* rendering */
} /* eps */
