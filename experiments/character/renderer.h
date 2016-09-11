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

#ifndef EXPERIMENTS_CHARATER_RENDERER_H_INCLUDED
#define EXPERIMENTS_CHARATER_RENDERER_H_INCLUDED

#include <elements/utils/std/pointer.h>
#include <elements/rendering/passes/pass_composition.h>
#include <elements/scene/scene.h>
#include <elements/scene/camera/camera.h>
#include <elements/scene/modifier/positioning.h>

namespace eps {
namespace experiment {
namespace character {

class renderer
{
public:

    renderer();

    bool initialize();
    bool construct(const math::uvec2 & size);

    void render(float dt);

    bool set_model(const std::string & model);
    void set_rotation(float theta, float phi);

private:

    class modifier_polar : public scene::modifier_positioning
    {
    public:

        using scene::modifier_positioning::modifier_positioning;

    public:

        void process(float dt) final;

        void set_radius(float value) { radius_ = value; }
        void set_theta(float value) { theta_ = value; }
        void set_phi(float value) { phi_ = value; }

    private:

        float radius_ = 1.0f;
        float theta_  = 0.0f;
        float phi_    = 0.0f;
    };

private:

    rendering::pass_composition passes_;
    utils::pointer<scene::scene> scene_;
    utils::link<scene::node> model_;
    utils::link<modifier_polar> modifier_camera_;
    utils::link<scene::modifier_positioning> modifier_light_;
};

} /* character */
} /* experiment */
} /* eps */

#endif // EXPERIMENTS_CHARATER_RENDERER_H_INCLUDED
