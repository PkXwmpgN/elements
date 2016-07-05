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

#ifndef RENDERING_TEXTURE_H_INCLUDED
#define RENDERING_TEXTURE_H_INCLUDED

#include "math/types.h"
#include "utils/std/product.h"
#include "opengl.h"

namespace eps {
namespace rendering {

template<typename _Texture_policy>
class texture : private _Texture_policy
{
public:

    using policy_type = _Texture_policy;

    texture();
    ~texture();

    texture(const texture &) = delete;
    texture & operator=(const texture &) = delete;
    texture(texture &&) = default;
    texture & operator=(texture &&) = default;

    void set_data(const void * data, const math::uvec2 & size, enum_type format);
    void set_data(const void * data, const math::uvec2 & size);

    const product_type & get_product() const;
    const math::uvec2 & get_size() const;

private:

    product_type product_;
    math::uvec2 size_;
};

template<typename _Texture_policy>
inline texture<_Texture_policy>::texture()
{
    glGenTextures(1, utils::ptr_product(product_));
    glBindTexture(GL_TEXTURE_2D, utils::raw_product(product_));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, policy_type::filter());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, policy_type::filter());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, policy_type::wrap());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, policy_type::wrap());
    glBindTexture(GL_TEXTURE_2D, 0);
}

template<typename _Texture_policy>
inline texture<_Texture_policy>::~texture()
{
    if(!product_.invalid())
        glDeleteTextures(1, utils::ptr_product(product_));
}

template<typename _Texture_policy>
inline void texture<_Texture_policy>::set_data(const void * data,
                                               const math::uvec2 & size,
                                               enum_type format)
{
    glBindTexture(GL_TEXTURE_2D, utils::raw_product(product_));
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y,
                 0, format, policy_type::type(), data);
    glBindTexture(GL_TEXTURE_2D, 0);
    size_ = size;
}

template<typename _Texture_policy>
inline void texture<_Texture_policy>::set_data(const void * data,
                                               const math::uvec2 & size)
{
    this->set_data(data, size, policy_type::format());
}

template<typename _Texture_policy>
inline const product_type & texture<_Texture_policy>::get_product() const
{
    return product_;
}

template<typename _Texture_policy>
inline const math::uvec2 & texture<_Texture_policy>::get_size() const
{
    return size_;
}

} /* rendering */
} /* eps */

#endif // RENDERING_TEXTURE_H_INCLUDED
