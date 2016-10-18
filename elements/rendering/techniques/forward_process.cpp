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


#include "forward_process.h"
#include "rendering/models/model_warehouse.h"
#include "rendering/state/state_macro.h"
#include "rendering/utils/program_loader.h"
#include "math/matrix.h"
#include "math/trigonometry.h"

namespace eps {
namespace rendering {
namespace techniques {

enum class program_enum : short
{
    // attributes
    a_vertex_pos = 0,
    a_vertex_normal = 1,
    a_vertex_tangent = 2,
    a_vertex_uv = 3,
    // uniforms
    u_matrix_mvp = 0,
    u_matrix_model_view = 1,
    u_matrix_normal = 2,
    u_matrix_view = 3,
    u_camera_pos = 4,
    u_map_diffuse = 5,
    u_map_specular = 6,
    u_map_normal = 7,
    u_light_pos = 8,
    u_light_intensity = 9,
    u_light_range = 10
};

struct lights_process : public scene::visitor<lights_process>
{
public:

    SNAPE_VISIT(scene::light_point);

    explicit lights_process(program * prog)
        : program_(prog)
    {}

public:

    void visit(const scene::light_point & light)
    {
        program_->uniform_value(utils::to_int(program_enum::u_light_intensity), light.get_intensity());
        program_->uniform_value(utils::to_int(program_enum::u_light_range), light.get_range());
        program_->uniform_value(utils::to_int(program_enum::u_light_pos),
                                scene::get_position(light.get_node()));

        // TODO: process all lights
    }

private:

    program * program_ = nullptr;
};

bool forward_process::initialize()
{
    return load_program("assets/shaders/techniques/forward.prog", program_);
}

void forward_process::set_scene(const utils::pointer<scene::scene> & scene)
{
    scene_ = scene;
}

void forward_process::process()
{
    if(scene_)
        scene_->process_entities(*this);
}

void forward_process::visit(const model & sm)
{
    assert(scene_);

    auto node = sm.get_node().lock();
    assert(node);

    auto camera = scene_->get_camera().lock();
    assert(camera);

    auto warehouse = sm.get_warehouse().lock();
    assert(warehouse);

    EPS_STATE_PROGRAM(program_.get_product());

    lights_process process(&program_);
    scene_->process_lights(process);

    program_.uniform_value(utils::to_int(program_enum::u_camera_pos),
                           scene::get_position(camera->get_node()));

    for(const auto & mesh : sm)
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

        if(const auto & texture = material.get_texture(scene::material::type_texture::diffuse))
        {
            EPS_STATE_SAMPLER_0(texture.value());
            program_.uniform_value(utils::to_int(program_enum::u_map_diffuse), 0);
        }

        if(const auto & texture = material.get_texture(scene::material::type_texture::specular))
        {
            EPS_STATE_SAMPLER_1(texture.value());
            program_.uniform_value(utils::to_int(program_enum::u_map_specular), 1);
        }

        if(const auto & texture = material.get_texture(scene::material::type_texture::normals))
        {
            EPS_STATE_SAMPLER_2(texture.value());
            program_.uniform_value(utils::to_int(program_enum::u_map_normal), 2);
        }

        const math::mat4 & mv = camera->get_view() * node->get_world_matrix();
        const math::mat4 & projection = camera->get_projection();
        program_.uniform_value(utils::to_int(program_enum::u_matrix_mvp), projection * mv);
        program_.uniform_value(utils::to_int(program_enum::u_matrix_model_view), mv);
        program_.uniform_value(utils::to_int(program_enum::u_matrix_view), camera->get_view());
        program_.uniform_value(utils::to_int(program_enum::u_matrix_normal),
                               math::transpose(math::inverse(math::mat3(mv))));

        EPS_STATE_INDICES(geometry.get_indices());
        glDrawElements(GL_TRIANGLES, geometry.get_indices_count(), GL_UNSIGNED_SHORT, 0);
    }
}


} /* techniques */
} /* rendering */
} /* eps */
