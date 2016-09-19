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

#include "renderer.h"
#include <elements/rendering/models/process_load_model.h>
#include <elements/rendering/effects/clear.h>
#include <elements/rendering/techniques/forward.h>
#include <elements/math/trigonometry.h>
#include <elements/math/transform.h>
#include <elements/assets/assets_storage.h>
#include <elements/assets/asset_model.h>

namespace eps {
namespace experiment {
namespace character {

void renderer::modifier_polar::process(float dt)
{
    math::vec3 pos = radius_ * math::vec3(math::sin(theta_) * math::sin(phi_),
                                          math::cos(theta_),
                                          math::sin(theta_) * math::cos(phi_));
    look_at(pos, math::vec3(0.0f, 0.0f, 0.0f), math::vec3(0.0f, 1.0f, 0.0f));
    modifier_positioning::process(dt);
}

renderer::renderer()
    : scene_(utils::make_shared<scene::scene>())
{}

bool renderer::initialize()
{
    // initialize camera
    auto node = scene_->add_node("Camera");
    auto camera = scene_->add_node_camera<scene::camera_perspective>(node);
    if(camera.expired())
        return false;

    modifier_camera_ = scene_->add_node_modifier<modifier_polar>(node);
    if(modifier_camera_.expired())
        return false;

    // initialize lights
    node = scene_->add_node("Light");
    auto light = scene_->add_node_light<scene::light_point>(node);
    if(light.expired())
        return false;

    light.lock()->set_range(100.0f);

    modifier_light_ = scene_->add_node_modifier<scene::modifier_positioning>(node);
    if(modifier_light_.expired())
        return false;

    // initialize passes
    using namespace rendering;
    passes_.initialize(2);

    auto link_clear = passes_.add_pass<effect::clear>();
    if(link_clear.expired())
        return false;

    auto link_character = passes_.add_pass<techniques::forward>();
    if(link_character.expired())
        return false;

    auto clear = link_clear.lock();
    clear->set_color(math::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    clear->set_bits(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto character = link_character.lock();
    character->set_scene(scene_);

    return true;
}

bool renderer::construct(const math::uvec2 & size)
{
    if(auto camera = scene_->get_camera().lock())
    {
        camera->set_aspect(math::aspect(size));
        camera->set_fov(math::radians(45.0f));
        camera->set_near(0.1f);
        camera->set_far(100.0f);
    }

    if(auto modifier = modifier_camera_.lock())
    {
        modifier->look_at(math::vec3(0.0f, 0.0f, 5.0f),
                          math::vec3(0.0f, 0.0f, 0.0f),
                          math::vec3(0.0f, 1.0f, 0.0f));
    }

    if(auto modifier = modifier_light_.lock())
    {
        modifier->set_position(math::vec3(20.0f, 20.0f, 20.0f));
    }

    passes_.construct(size);
    return true;
}

void renderer::render(float dt)
{
    if(scene_)
    {
        scene_->update(dt);
        passes_.process(dt);
    }
}

bool renderer::set_model(const std::string & model)
{
    if(auto m = model_.lock())
        m->set_state(scene::node::state::dead);

    // load an asset
    auto asset = assets_storage::instance().read<asset_model>(model);
    if(!asset)
        return false;

    // attach the model hierarchy to the scene
    auto node = asset->get_hierarchy();
    scene_->get_root().lock()->attach_node(node);

    // load entities to rendering
    node->process(rendering::process_load_model(scene_), std::cref(asset.value()));

    model_ = node;
    return true;
}

void renderer::set_rotation(float theta, float phi)
{
    if(auto modifier = modifier_camera_.lock())
    {
        modifier->set_theta(theta);
        modifier->set_phi(phi);
        modifier->set_radius(5.0f);
    }
}

} /* character */
} /* experiment */
} /* eps */
