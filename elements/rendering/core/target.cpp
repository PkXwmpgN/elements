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

#include "target.h"

namespace eps {
namespace rendering {

target::target()
{
    glGenFramebuffers(1, utils::ptr_product(product_));
}

target::~target()
{
    if(!product_.invalid())
        glDeleteFramebuffers(1, utils::ptr_product(product_));
}

bool target::attach(attachment id, texture data)
{
    static enum_type gl_attachments[] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };

    attachments_[utils::to_int(id)] = std::move(data);

    GLint save;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &save);
    glBindFramebuffer(GL_FRAMEBUFFER, utils::raw_product(product_));

    glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachments[utils::to_int(id)], GL_TEXTURE_2D,
                           utils::raw_product(attachments_[utils::to_int(id)].get_product()), 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    glBindFramebuffer(GL_FRAMEBUFFER, save);
    return status == GL_FRAMEBUFFER_COMPLETE;
}

const product_type & target::get_target() const
{
    return product_;
}

const product_type & target::get_product() const
{
    return get_product(attachment::color0);
}

const math::uvec2 & target::get_size() const
{
    return get_size(attachment::color0);
}

const product_type & target::get_product(attachment id) const
{
    return attachments_[utils::to_int(id)].get_product();
}

const math::uvec2 & target::get_size(attachment id) const
{
    return attachments_[utils::to_int(id)].get_size();
}

} /* rendering */
} /* eps */
