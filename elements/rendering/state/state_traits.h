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

#ifndef RENDERING_STATE_STATE_TRAITS_H_INCLUDED
#define RENDERING_STATE_STATE_TRAITS_H_INCLUDED

#include "math/types.h"
#include "utils/std/product.h"
#include "rendering/core/opengl.h"

namespace eps {
namespace rendering {

struct state_viewport;
struct state_blend;
struct state_target;
struct state_sampler;
struct state_vertices;
struct state_indices;
struct state_program;

template<>
struct state_traits<state_viewport>
{
    using value_type = GLint[4];

    static void init(value_type & result)
    {
        glGetIntegerv(GL_VIEWPORT, result);
    }

    static void restore(const value_type & value)
    {
        glViewport(value[0], value[1], value[2], value[3]);
    }

    static void set(const math::uvec2 & size)
    {
        glViewport(0, 0, size.x, size.y);
    }

    static void set(GLint x, GLint y, GLsizei cx, GLsizei cy)
    {
        glViewport(x, y, cx, cy);
    }
};

template<>
struct state_traits<state_blend>
{
    using value_type = GLboolean;

    static void init(value_type & result)
    {
        result = glIsEnabled(GL_BLEND);
    }

    static void restore(const value_type & value)
    {
        if(value == GL_FALSE)
            glDisable(GL_BLEND);
    }

    static void set(GLenum src_rgb, GLenum dst_rgb, GLenum src_alpha, GLenum dst_alpha)
    {
        glEnable(GL_BLEND);
        glBlendFuncSeparate(src_rgb, dst_rgb, src_alpha, dst_alpha);
    }

    static void set(GLenum sfactor, GLenum dfactor)
    {
        glEnable(GL_BLEND);
        glBlendFunc(sfactor, dfactor);
    }
};

template<>
struct state_traits<state_target>
{
    using value_type = GLint;

    static void init(value_type & result)
    {
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &result);
    }

    static void restore(const value_type & value)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, value);
    }

    static void set(const product_type & product)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, utils::raw_product(product));
    }
};

template<>
struct state_traits<state_sampler>
{
    using value_type = GLuint;

    static void init(value_type & result)
    {
        result = 0;
    }

    static void restore(const value_type & value)
    {
        glBindTexture(GL_TEXTURE_2D, value);
    }

    static void set(const product_type & product, GLenum active)
    {
        glActiveTexture(GL_TEXTURE0 + active);
        glBindTexture(GL_TEXTURE_2D, utils::raw_product(product));
    }
};

template<>
struct state_traits<state_vertices>
{
    using value_type = GLuint;

    static void init(value_type & result)
    {
        result = 0;
    }

    static void restore(const value_type & value)
    {
        glBindBuffer(GL_ARRAY_BUFFER, value);
    }

    static void set(const product_type & product)
    {
        glBindBuffer(GL_ARRAY_BUFFER, utils::raw_product(product));
    }
};

template<>
struct state_traits<state_indices>
{
    using value_type = GLuint;

    static void init(value_type & result)
    {
        result = 0;
    }

    static void restore(const value_type & value)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, value);
    }

    static void set(const product_type & product)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, utils::raw_product(product));
    }
};

template<>
struct state_traits<state_program>
{
    using value_type = GLuint;

    static void init(value_type & result)
    {
        result = 0;
    }

    static void restore(const value_type & value)
    {
        glUseProgram(value);
    }

    static void set(const product_type & product)
    {
        glUseProgram(utils::raw_product(product));
    }
};

} /* rendering */
} /* eps */

#endif // RENDERING_STATE_STATE_TRAITS_H_INCLUDED
