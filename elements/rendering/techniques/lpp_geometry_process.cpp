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

#include "lpp_geometry_process.h"
#include "rendering/mesh/mesh_storage.h"
#include "rendering/state/state_macro.h"
#include "rendering/utils/program_loader.h"
#include "utils/std/enum.h"
#include "math/trigonometry.h"

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
    u_matrix_model_view = 1,
    u_map_normal = 2,
    u_has_map_normal = 3
};

bool lpp_geometry_process::initialize()
{
    return load_program("assets/shaders/techniques/lpp_geometry.prog", program_);
}

void lpp_geometry_process::process()
{
    if(scene_)
        scene_->process_entities(*this);
}

void lpp_geometry_process::visit(const mesh & sm)
{
    assert(scene_);

    auto node = sm.get_node().lock();
    assert(node);

    auto camera = scene_->get_camera().lock();
    assert(camera);

    auto storage = sm.get_storage().lock();
    assert(storage);

    EPS_STATE_PROGRAM(program_.get_product());

    const auto & geometry = storage->get_geometry(sm.get_storage_branch(mesh::branch::geometry));

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

    const auto & maps = storage->get_maps(sm.get_storage_branch(mesh::branch::maps));

    if(const auto & map = maps.get_normal())
    {
        EPS_STATE_SAMPLER_0(map.value());
        program_.uniform_value(utils::to_int(program_enum::u_map_normal), 0);
        program_.uniform_value(utils::to_int(program_enum::u_has_map_normal), true);
    }
    else
    {
        program_.uniform_value(utils::to_int(program_enum::u_has_map_normal), false);
    }
    
    const math::mat4 & mv = camera->get_view() * node->get_world_matrix();
    const math::mat4 & projection = camera->get_projection();
    program_.uniform_value(utils::to_int(program_enum::u_matrix_mvp), projection * mv);
    program_.uniform_value(utils::to_int(program_enum::u_matrix_model_view), mv);

    EPS_STATE_INDICES(geometry.get_indices());
    glDrawElements(GL_TRIANGLES, geometry.get_indices_count(), GL_UNSIGNED_SHORT, 0);
}

} /* rendering */
} /* eps */
