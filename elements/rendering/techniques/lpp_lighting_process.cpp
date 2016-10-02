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

#include "lpp_lighting_process.h"
#include "rendering/state/state_macro.h"
#include "rendering/utils/program_loader.h"
#include "utils/std/enum.h"
#include "math/transform.h"
#include "math/trigonometry.h"

namespace eps {
namespace rendering {

enum class program_enum : short
{
    // attributes
    a_vertex_pos = 0,
    // uniforms
    u_matrix_mvp = 0,
    u_camera_view_param = 1,
    u_camera_near = 2,
    u_camera_far = 3,
    u_map_geometry = 4,
    u_map_depth = 5,
    u_light_pos = 6,
    u_light_intensity = 7,
    u_light_inv_range_square = 8,
};

enum class program_stencil_enum : short
{
    // attributes
    a_vertex_pos = 0,
    // uniforms
    u_matrix_mvp = 0
};

lpp_lighting_process::lpp_lighting_process()
    : sphere_(2)
{}

bool lpp_lighting_process::initialize()
{
    return load_program("assets/shaders/techniques/lpp_lighting.prog", program_) &&
           load_program("assets/shaders/techniques/lpp_lighting_stencil.prog", program_stencil_);
}

void lpp_lighting_process::stencil(const math::mat4 & mvp)
{
    EPS_STATE_DEPTH_TEST();
    EPS_STATE_COLOR_READONLY();

    // TODO: stencil settings wrapper
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_ALWAYS, 0, 0);
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

    EPS_STATE_PROGRAM(program_stencil_.get_product());

    program_stencil_.uniform_value(utils::to_int(program_stencil_enum::u_matrix_mvp), mvp);
    sphere_.render(program_stencil_, utils::to_int(program_stencil_enum::a_vertex_pos));

    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
}

void lpp_lighting_process::visit(const scene::light_point & light, scene::scene & scene)
{
    auto node = light.get_node().lock();
    assert(node);

    auto camera = scene.get_camera().lock();
    assert(camera);

    math::mat4 transform = camera->get_projection() * camera->get_view() * node->get_world_matrix() *
        math::scale(light.get_range(), light.get_range(), light.get_range());

    stencil(transform);

    EPS_STATE_CULLFACE();
    EPS_STATE_CULLFACE_MODE(GL_FRONT);
    EPS_STATE_BLEND(GL_ONE, GL_ONE);
    EPS_STATE_PROGRAM(program_.get_product());
    EPS_STATE_SAMPLER_0(map_geometry_);
    EPS_STATE_SAMPLER_1(map_depth_);

    program_.uniform_value(utils::to_int(program_enum::u_map_geometry), 0);
    program_.uniform_value(utils::to_int(program_enum::u_map_depth), 1);
    program_.uniform_value(utils::to_int(program_enum::u_matrix_mvp), transform);

    const float tan_half_fov = math::tan(camera->get_fov() / 2.0f);
    math::vec2 camera_view_param = math::vec2(tan_half_fov * camera->get_aspect(), tan_half_fov);

    program_.uniform_value(utils::to_int(program_enum::u_camera_view_param), camera_view_param);
    program_.uniform_value(utils::to_int(program_enum::u_camera_near), camera->get_near());
    program_.uniform_value(utils::to_int(program_enum::u_camera_far), camera->get_far());

    const float inv_range_square = 1.0f / (light.get_range() * light.get_range());
    math::vec3 light_pos(camera->get_view() * math::vec4(scene::get_position(light.get_node()), 1.0f));

    program_.uniform_value(utils::to_int(program_enum::u_light_pos), light_pos);
    program_.uniform_value(utils::to_int(program_enum::u_light_intensity), light.get_intensity());
    program_.uniform_value(utils::to_int(program_enum::u_light_inv_range_square), inv_range_square);

    sphere_.render(program_, utils::to_int(program_enum::a_vertex_pos));
}

} /* rendering */
} /* eps */
