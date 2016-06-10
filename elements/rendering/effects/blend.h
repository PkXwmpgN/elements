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

#ifndef RENDERING_BLEND_H_INCLUDED
#define RENDERING_BLEND_H_INCLUDED

#include "math/types.h"
#include "rendering/core/program.h"
#include "rendering/passes/pass_base.h"
#include "rendering/primitives/square.h"

namespace eps {
namespace rendering {
namespace effect {

class blend : public pass_base
{
public:

    using pass_base::pass_base;
    void set_factors(enum_type sfactor, enum_type dfactor);

public:

    bool initialize() final;
    void process(float dt) final;

private:

    program program_;
    primitive::square square_;

    enum_type sfactor_ = GL_SRC_ALPHA;
    enum_type dfactor_ = GL_ONE_MINUS_SRC_ALPHA;
};

} /* effect */
} /* rendering */
} /* eps */

#endif // RENDERING_BLEND_H_INCLUDED
