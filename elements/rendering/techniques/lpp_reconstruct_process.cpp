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

#include "lpp_reconstruct_process.h"
#include "rendering/mesh/mesh_storage.h"
#include "rendering/state/state_macro.h"
#include "rendering/utils/program_loader.h"
#include "utils/std/enum.h"

namespace eps {
namespace rendering {

enum class program_enum : short
{
    // attributes
    a_vertex_pos = 0,
    a_vertex_uv = 1,
    // uniforms
    u_matrix_mvp = 0,
    u_map_light = 1,
    u_map_diffuse = 2,
    u_map_specular = 3,
    u_has_map_diffuse = 4,
    u_has_map_specular = 5,
    u_color_diffuse = 6,
    u_color_specular = 7
};

bool lpp_reconstruct_process::initialize()
{
    return load_program("assets/shaders/techniques/lpp_reconstruct.prog", program_);
}

void lpp_reconstruct_process::process()
{
    if(scene_)
        scene_->process_entities(*this);
}

void lpp_reconstruct_process::visit(const mesh & sm)
{
    assert(scene_);

    auto node = sm.get_node().lock();
    assert(node);

    auto camera = scene_->get_camera().lock();
    assert(camera);

    auto storage = sm.get_storage().lock();
    assert(storage);

    EPS_STATE_PROGRAM(program_.get_product());

    EPS_STATE_SAMPLER_0(map_light_);
    program_.uniform_value(utils::to_int(program_enum::u_map_light), 0);

    const auto & geometry = storage->get_geometry(sm.get_storage_branch(mesh::branch::geometry));
    EPS_STATE_VERTICES(geometry.get_vertices());

    program_.attribute_array_enable(utils::to_int(program_enum::a_vertex_pos));
    program_.attribute_array(utils::to_int(program_enum::a_vertex_pos),
                             offsetof(scene::vertex, position), 3, sizeof(scene::vertex));

    program_.attribute_array_enable(utils::to_int(program_enum::a_vertex_uv));
    program_.attribute_array(utils::to_int(program_enum::a_vertex_uv),
                             offsetof(scene::vertex, tex), 2, sizeof(scene::vertex));

    const auto & maps = storage->get_maps(sm.get_storage_branch(mesh::branch::maps));
    const auto & colors = storage->get_colors(sm.get_storage_branch(mesh::branch::colors));

    if(const auto & map = maps.get_diffuse())
    {
        EPS_STATE_SAMPLER_1(map.value());
        program_.uniform_value(utils::to_int(program_enum::u_map_diffuse), 1);
        program_.uniform_value(utils::to_int(program_enum::u_has_map_diffuse), true);
    }
    else if(const auto & color = colors.get_diffuse())
    {
        program_.uniform_value(utils::to_int(program_enum::u_color_diffuse), color.value());
        program_.uniform_value(utils::to_int(program_enum::u_has_map_diffuse), false);
    }

    if(const auto & map = maps.get_specular())
    {
        EPS_STATE_SAMPLER_2(map.value());
        program_.uniform_value(utils::to_int(program_enum::u_map_specular), 2);
        program_.uniform_value(utils::to_int(program_enum::u_has_map_specular), true);
    }
    else if(const auto & color = colors.get_specular())
    {
        program_.uniform_value(utils::to_int(program_enum::u_color_specular), color.value());
        program_.uniform_value(utils::to_int(program_enum::u_has_map_diffuse), false);
    }

    EPS_STATE_VERTICES(geometry.get_vertices());

    program_.uniform_value(utils::to_int(program_enum::u_matrix_mvp),
                           camera->get_view_projection() * node->get_world_matrix());

    EPS_STATE_INDICES(geometry.get_indices());
    glDrawElements(GL_TRIANGLES, geometry.get_indices_count(), GL_UNSIGNED_SHORT, 0);
    
}

} /* rendering */
} /* eps */
