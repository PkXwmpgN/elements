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
#include <elements/rendering/mesh/mesh_load_process.h>
#include <elements/rendering/effects/clear.h>
#include <elements/rendering/techniques/lpp.h>
#include <elements/rendering/techniques/forward.h>
#include <elements/scene/camera/camera_load_process.h>
#include <elements/math/trigonometry.h>
#include <elements/math/transform.h>
#include <elements/math/quaternion.h>
#include <elements/math/common.h>
#include <elements/assets/assets_storage.h>
#include <elements/assets/asset_scene.h>
#include <elements/metrics/metrics.h>
#include <elements/ui/controls/check.h>

namespace eps {
namespace experiment {
namespace demo {

struct init_light_process : public scene::visitor<init_light_process>
{
public:

    SNAPE_VISIT(scene::light_point);

public:

    const std::set<std::string> & get_lights() const { return lights_; }

    void visit(const scene::light_point & light)
    {
        if(auto node = light.get_node().lock())
        {
            lights_.insert(node->get_name());
        }
    }

private:

    std::set<std::string> lights_;
};

struct activate_light_process : public scene::visitor<activate_light_process>
{
public:

    SNAPE_VISIT(scene::light_point);

public:

    explicit activate_light_process(bool activate)
        : activate_(activate)
    {}

    void visit(scene::light_point & light)
    {
        light.set_enabled(activate_);
    }

private:

    bool activate_;
};

void renderer::modifier_polar::process(float dt)
{
    const math::vec3 up = math::vec3(0.0f, 1.0f, 0.0f);
    const math::vec3 direction = math::vec3(math::sin(theta_) * math::sin(phi_),
                                            math::cos(theta_),
                                            math::sin(theta_) * math::cos(phi_));

    set_rotation(math::slerp(get_rotation(), math::look_rotation(direction, up), dt * 5.0f));

    modifier_positioning::process(dt);
}

void renderer::modifier_polar::set_theta(float value)
{
    const float min = math::radians(70.0f);
    const float max = math::radians(90.0f);
    theta_ = math::clamp(theta_ + (value - theta_control_), min, max);
    theta_control_ = value;
}
void renderer::modifier_polar::set_phi(float value)
{
    const float min = math::radians(-10.0f);
    const float max = math::radians(10.0f);
    phi_ = math::clamp(phi_ + (value - phi_control_), min, max);
    phi_control_ = value;
}

renderer::renderer()
    : scene_(utils::make_shared<scene::scene>())
{}

bool renderer::initialize()
{
    if(!initialize_passes())
        return false;

    if(!initialize_camera())
        return false;

    if(!initialize_scene())
        return false;

    return true;
}

bool renderer::initialize_passes()
{
    using namespace rendering;
    passes_.initialize(2);

    auto link_clear = passes_.add_pass<effect::clear>();
    if(link_clear.expired())
        return false;

    auto link_render = passes_.add_pass<techniques::lpp>();
    if(link_render.expired())
        return false;

    auto clear = link_clear.lock();
    clear->set_color(math::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    clear->set_bits(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto render = link_render.lock();
    render->set_scene(scene_);

    return true;
}

bool renderer::initialize_camera()
{
    auto node = scene_->add_node("Camera");
    auto camera = scene_->add_node_camera<scene::camera_perspective>(node);
    if(camera.expired())
        return false;

    modifier_camera_ = scene_->add_node_modifier<modifier_polar>(node);
    if(modifier_camera_.expired())
        return false;

    return true;
}

bool renderer::initialize_scene()
{
    auto asset = assets_storage::instance().read<asset_scene>("assets/space.dae");
    if(!asset)
        return false;

    auto hierarchy = asset->get_hierarchy();
    scene_->get_root().lock()->attach_node(hierarchy);
    hierarchy->process(rendering::mesh_load_process(scene_), asset.value());
    hierarchy->process(scene::camera_load_process(scene_), asset.value());

    init_light_process iprocess;
    scene_->process_lights(iprocess);
    lights_ = iprocess.get_lights();

    activate_light_process aprocess(false);
    scene_->process_lights(aprocess);

    return true;
}

bool renderer::initialize_ui(const math::uvec2 & size)
{
    ui_ = utils::make_unique<ui::system>();
    ui_->construct(size);

    using namespace metric_literals;

    float offset = 8_dp;
    for(const auto & name : lights_)
    {
        if(auto button = ui_->add<ui::check>().lock())
        {
            button->set_asset("assets/ui/textures/light.png");
            button->set_size(32_dp, 32_dp);
            button->set_position(offset, 8_dp);
            button->set_click([this, name](ui::state state)
            {
                this->activate_light(name, state == ui::state::PRESSED);
            });

            offset += 40_dp;
        }
    }

    label_fps_ = ui_->add<eps::ui::label>();
    if(auto label = label_fps_.lock())
    {
        label->set_size(128_dp, 18_dp);
        label->set_position(8_dp, size.y - 32_dp);
        label->set_font("assets/ui/fonts/OpenSans-Regular.ttf", std::floor(18_dp));
        label->set_color(eps::math::vec4(0.8f, 0.8f, 0.8f, 1.0f));
    }

    return true;
}

void renderer::activate_light(const std::string & name, bool activate)
{
    if(scene_)
    {
        activate_light_process process(activate);
        scene_->process_light(name, process);
    }
}

bool renderer::construct(const math::uvec2 & size)
{
    if(auto camera = scene_->get_camera().lock())
    {
        camera->set_aspect(math::aspect(size));
        camera->set_fov(math::radians(60.0f));
        camera->set_near(0.1f);
        camera->set_far(50.0f);
    }

    if(auto modifier = modifier_camera_.lock())
    {
        modifier->set_position(math::vec3(0.0f, 6.0f, 14.0f));
        modifier->set_theta(math::radians(75.0f));
        modifier->set_phi(math::radians(0.0f));
    }

    passes_.construct(size);
    initialize_ui(size);

    return true;
}

void renderer::touch(const math::uvec2 & pos, ui::touch_action action, ui::touch_finger finger)
{
    if(ui_)
        ui_->touch(pos.x, pos.y, action, finger);
}

void renderer::render(float dt)
{
    if(scene_)
    {
        scene_->update(dt);
        passes_.process(dt);

        if(ui_)
            ui_->draw();
    }
}

void renderer::set_rate(float rate)
{
    if(ui_)
    {
        if(auto label = label_fps_.lock())
        {
            static char text[64] = "";
            sprintf(text, "FPS: %.2f", rate);
            label->set_text(text);
        }
    }
}

void renderer::set_rotation(float theta, float phi)
{
    if(auto modifier = modifier_camera_.lock())
    {
        modifier->set_theta(theta);
        modifier->set_phi(phi);
    }
}

} /* demo */
} /* experiment */
} /* eps */
