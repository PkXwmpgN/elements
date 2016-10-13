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

#ifndef RENDERING_TEXTURE_MAKER_H_INCLUDED
#define RENDERING_TEXTURE_MAKER_H_INCLUDED

#include "texture.h"
#include "opengl.h"

namespace eps {
namespace rendering {

struct texture_maker
{
    virtual texture construct(const void * data, const math::uvec2 & size) const = 0;
};

template<typename _Policy>
struct texture_maker_policy : public texture_maker
{
    texture construct(const void * data, const math::uvec2 & size) const final;
};

template<typename _Policy>
inline texture_maker_policy<_Policy> get_texture_maker()
{
    return texture_maker_policy<_Policy>();
}

template<typename _Policy>
texture texture_maker_policy<_Policy>::construct(const void * data,
                                                 const math::uvec2 & size) const
{
    texture result(size);
    glBindTexture(GL_TEXTURE_2D, utils::raw_product(result.get_product()));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _Policy::filter());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _Policy::filter());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _Policy::wrap());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _Policy::wrap());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, _Policy::internal_format(),
                 result.get_size().x, result.get_size().y, 0,
                 _Policy::format(), _Policy::type(), data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return result;
}

} /* rendering */
} /* eps */

#endif // RENDERING_TEXTURE_MAKER_H_INCLUDED
