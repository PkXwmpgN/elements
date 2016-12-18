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

#include "projector_process.h"
#include "rendering/core/texture_maker.h"
#include "rendering/core/texture_policy.h"
#include "rendering/state/state_macro.h"
#include "rendering/utils/program_loader.h"
#include "rendering/mesh/mesh_storage.h"
#include "assets/assets_storage.h"
#include "assets/asset_texture.h"

namespace eps {
namespace rendering {
namespace techniques {

enum class program_enum : short
{
    // attributes
    a_vertex_pos = 0,
    // uniforms
    u_matrix_mvp = 0,
    u_matrix_model = 1,
    u_matrix_pvp = 2,
    u_map_projective = 3
};

bool projector_process::initialize()
{
    return load_program("assets/shaders/techniques/projector.prog", program_);
}

void projector_process::set_scene(const utils::pointer<scene::scene> & scene)
{
    scene_ = scene;
}

void projector_process::process()
{
    if(scene_)
        scene_->process_entities(*this);
}

bool projector_process::set_projective_map(const std::string & name)
{
    auto asset = assets_storage::instance().read<asset_texture>(name);
    if(asset)
    {
        auto maker = get_texture_maker<repeat_texture_policy>();
        texture_projective_ = maker.construct(asset->pixels(), asset->size());
        return true;
    }
    return false;
}

void projector_process::set_projective_camera(const std::string & camera)
{
    camera_projective_ = camera;
}

void projector_process::visit(const mesh & sm)
{
    assert(scene_);

    auto node = sm.get_node().lock();
    assert(node);

    auto camera = scene_->get_camera().lock();
    assert(camera);

    auto camera_projective = scene_->get_camera(camera_projective_).lock();
    assert(camera_projective);

    auto storage = sm.get_storage().lock();
    assert(storage);

    EPS_STATE_PROGRAM(program_.get_product());

    EPS_STATE_SAMPLER_0(texture_projective_.get_product());
    program_.uniform_value(utils::to_int(program_enum::u_map_projective), 0);

    const auto & geometry = storage->get_geometry(sm.get_storage_branch(mesh::branch::geometry));
    EPS_STATE_VERTICES(geometry.get_vertices());

    program_.attribute_array_enable(utils::to_int(program_enum::a_vertex_pos));
    program_.attribute_array(utils::to_int(program_enum::a_vertex_pos),
                             offsetof(scene::vertex, position), 3, sizeof(scene::vertex));

    const math::mat4 & vp = camera->get_view_projection();
    const math::mat4 & pvp = camera_projective->get_view_projection();
    const math::mat4 & m  = node->get_world_matrix();

    program_.uniform_value(utils::to_int(program_enum::u_matrix_mvp), vp * m);
    program_.uniform_value(utils::to_int(program_enum::u_matrix_model), m);
    program_.uniform_value(utils::to_int(program_enum::u_matrix_pvp), pvp);

    EPS_STATE_INDICES(geometry.get_indices());
    glDrawElements(GL_TRIANGLES, geometry.get_indices_count(), GL_UNSIGNED_SHORT, 0);
}

} /* techniques */
} /* rendering */
} /* eps */
