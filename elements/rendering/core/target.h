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

#ifndef RENDERING_TARGET_H_INCLUDED
#define RENDERING_TARGET_H_INCLUDED

#include "opengl.h"
#include "texture.h"
#include "math/types.h"

namespace eps {
namespace rendering {

template<typename _Attachement_policy>
class target : private _Attachement_policy
{
public:

    using texture_type = texture<_Attachement_policy>;

    target();
    ~target();

    target(target &&) = default;
    target & operator=(target &&) = default;

    bool construct(const math::uvec2 & size);

    const product_type & get_target() const;
    const product_type & get_product() const;

    const math::uvec2 & get_size() const;

private:

    product_type product_;
    texture_type attachement_;
};

template<typename _Attachement_policy>
inline target<_Attachement_policy>::target()
{
    glGenFramebuffers(1, utils::ptr_product(product_));
}

template<typename _Attachement_policy>
inline target<_Attachement_policy>::~target()
{
    glDeleteFramebuffers(1, utils::ptr_product(product_));
}

template<typename _Attachement_policy>
inline bool target<_Attachement_policy>::construct(const math::uvec2 & size)
{
    attachement_.set_data(nullptr, size);

    GLint save;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &save);
    glBindFramebuffer(GL_FRAMEBUFFER, utils::raw_product(product_));
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           utils::raw_product(attachement_.get_product()), 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    glBindFramebuffer(GL_FRAMEBUFFER, save);

    return status == GL_FRAMEBUFFER_COMPLETE;
}
template<typename _Attachement_policy>
inline const product_type & target<_Attachement_policy>::get_target() const
{
    return product_;
}

template<typename _Attachement_policy>
inline const product_type & target<_Attachement_policy>::get_product() const
{
    return attachement_.get_product();
}

template<typename _Attachement_policy>
inline const math::uvec2 & target<_Attachement_policy>::get_size() const
{
    return attachement_.get_size();
}

} /* rendering */
} /* eps */

#endif // RENDERING_TARGET_H_INCLUDED
