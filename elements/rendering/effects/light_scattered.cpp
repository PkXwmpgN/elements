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

#include "light_scattered.h"
#include "blend.h"
#include "rendering/state/state_macro.h"
#include "rendering/passes/pass_target.h"
#include "rendering/core/program.h"
#include "rendering/core/texture_policy.h"
#include "rendering/primitives/square.h"
#include "rendering/utils/program_loader.h"
#include "utils/std/enum.h"

namespace eps {
namespace rendering {
namespace effect {

// ----------------------------------------
// pass light scattered

enum class enum_ls : short
{
    // attributes
    a_vertex_xy = 0,
    a_vertex_uv = 1,
    // uniforms
    u_occluding      = 0,
    u_exposure       = 1,
    u_decay          = 2,
    u_density        = 3,
    u_weight         = 4,
    u_light_position = 5,
};

class pass_ls : public pass_base
{
public:

    pass_ls(size_t place, const pass_target_input & inputs)
        : pass_base(place, inputs)
        , exposure_(0.5f)
        , decay_(0.95f)
        , density_(0.1f)
        , weight_(0.45f)
    {}

    void set_light(const math::vec2 & pos) { light_ = pos; }

    void set_exposure(float value) { exposure_ = value; }
    void set_decay(float value) { decay_ = value; }
    void set_density(float value) { density_ = value; }
    void set_weight(float value) { weight_ = value; }

public:

    bool initialize() final
    {
        return load_program("assets/shaders/effects/light_scattered.prog", program_);
    }

    void process(float) final
    {
        EPS_STATE_SAMPLER_0(get_inputs().get_slot(pass_slot::slot_0));
        EPS_STATE_PROGRAM(program_.get_product());

        program_.uniform_value(utils::to_int(enum_ls::u_occluding), 0);
        program_.uniform_value(utils::to_int(enum_ls::u_light_position), light_);
        program_.uniform_value(utils::to_int(enum_ls::u_exposure), exposure_);
        program_.uniform_value(utils::to_int(enum_ls::u_decay), decay_);
        program_.uniform_value(utils::to_int(enum_ls::u_density), density_);
        program_.uniform_value(utils::to_int(enum_ls::u_weight), weight_);

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        square_.render(program_, utils::to_int(enum_ls::a_vertex_xy),
                                 utils::to_int(enum_ls::a_vertex_uv));
    }

private:

    program program_;
    primitive::square square_;

    math::vec2 light_;

    float exposure_;
    float decay_;
    float density_;
    float weight_;
};

// ----------------------------------------
// light scattered effect

void light_scattered::set_downsample(size_t value)
{
    downsample_ = value;
}

bool light_scattered::initialize()
{
    passes_.initialize(4);

    auto link_ls_0 = passes_.add_pass<pass_ls>(get_inputs());
    if(link_ls_0.expired())
        return false;

    auto link_ls_1 = passes_.add_pass<pass_ls>();
    if(link_ls_1.expired())
        return false;

    auto link_ls_2 = passes_.add_pass<pass_ls>();
    if(link_ls_2.expired())
        return false;

    auto link_blend = passes_.add_pass<blend>(get_inputs());
    if(link_blend.expired())
        return false;

    passes_.add_dependency(link_ls_1, link_ls_0, pass_slot::slot_0);
    passes_.add_dependency(link_ls_2, link_ls_1, pass_slot::slot_0);

    link_ls_0.lock()->set_density(0.3f);
    link_ls_1.lock()->set_density(0.4f);
    link_ls_2.lock()->set_density(0.5f);

    return true;
}

void light_scattered::process(float dt)
{
    passes_.process(dt);
}

utils::unique<pass_target> light_scattered::construct(const math::uvec2 & size)
{
    if(downsample_)
    {
        const math::uvec2 down_size = size / downsample_;
        passes_.construct(down_size);
        return get_pass_target<default_texture_policy>(down_size);
    }

    passes_.construct(size);
    return nullptr;
}

} /* effect */
} /* rendering */
} /* eps */
