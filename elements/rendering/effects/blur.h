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

#ifndef RENDERING_BLUR_H_INCLUDED
#define RENDERING_BLUR_H_INCLUDED

#include "rendering/passes/pass_base.h"
#include "rendering/passes/pass_composition.h"
#include "rendering/core/program.h"
#include "rendering/primitives/square.h"

namespace eps {
namespace rendering {
namespace effect {

class blur_base : public pass_base
{
public:

    using pass_base::pass_base;
    void set_strong(float value);

public:

    bool initialize() final;
    void process(float dt) final;

    utils::unique<pass_target> construct(const math::uvec2 & size) final;

private:

    virtual math::vec2 offset(float strong, const math::vec2 & texel) const = 0;

private:

    program program_;
    primitive::square square_;

    math::vec2 texel_;
    float strong_ = 1.0f;
};

class blur_v : public blur_base
{
public:

    using blur_base::blur_base;

private:

    math::vec2 offset(float strong, const math::vec2 & texel) const final;
};

class blur_h : public blur_base
{
public:

    using blur_base::blur_base;

private:

    math::vec2 offset(float strong, const math::vec2 & texel) const final;
};

class blur : public pass_base
{
public:

    using pass_base::pass_base;

    void set_strong(float value);
    void set_downsample(size_t value);

public:

    bool initialize() final;
    void process(float dt) final;

    utils::unique<pass_target> construct(const math::uvec2 & size) final;

private:

    pass_composition passes_;
    utils::link<blur_h> link_blur_h_;
    utils::link<blur_v> link_blur_v_;

    size_t downsample_ = 0;
};

} /* effect */
} /* rendering */
} /* eps */

#endif // RENDERING_BLUR_H_INCLUDED
