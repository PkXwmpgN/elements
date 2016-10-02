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

#ifndef ELEMENTS_TECHNIQUES_LPP_H_INCLUDED
#define ELEMENTS_TECHNIQUES_LPP_H_INCLUDED

#include "rendering/passes/pass_base.h"
#include "rendering/passes/pass_composition.h"
#include "scene/scene.h"

namespace eps {
namespace rendering {
namespace techniques {

class lpp_geometry_pass;
class lpp_lighting_pass;
class lpp_reconstruct_pass;

class lpp : public pass_base
{
public:

    using pass_base::pass_base;

public:

    bool initialize() final;
    void process(float dt) final;
    utils::unique<pass_target> construct(const math::uvec2 & size) final;

    void set_scene(const utils::pointer<scene::scene> & scene);

private:

    pass_composition passes_;
    utils::link<lpp_geometry_pass> link_geometry_;
    utils::link<lpp_lighting_pass> link_lighting_;
    utils::link<lpp_reconstruct_pass> link_reconstruct_;
};

} /* techniques */
} /* rendering */
} /* eps */


#endif // ELEMENTS_TECHNIQUES_LPP_H_INCLUDED
