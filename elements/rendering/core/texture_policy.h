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

#ifndef RENDERING_TEXTURE_POLICY_H_INCLUDED
#define RENDERING_TEXTURE_POLICY_H_INCLUDED

#include "opengl.h"

namespace eps {
namespace rendering {

template
<
    enum_type _Internal_format,
    enum_type _Format,
    enum_type _Type,
    enum_type _Filter,
    enum_type _Wrap
>
struct texture_policy
{
    static constexpr enum_type internal_format() { return _Internal_format; }
    static constexpr enum_type format() { return _Format; }
    static constexpr enum_type type() { return _Type; }
    static constexpr enum_type filter() { return _Filter; }
    static constexpr enum_type wrap() { return _Wrap; }
};

using default_texture_policy = texture_policy
<
    GL_RGBA,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    GL_LINEAR,
    GL_CLAMP_TO_EDGE
>;

using alpha_texture_policy = texture_policy
<
    GL_ALPHA,
    GL_ALPHA,
    GL_UNSIGNED_BYTE,
    GL_LINEAR,
    GL_CLAMP_TO_EDGE
>;

using repeat_texture_policy = texture_policy
<
    GL_RGBA,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    GL_LINEAR,
    GL_REPEAT
>;

using depth_texture_policy = texture_policy
<
    GL_DEPTH_COMPONENT16,
    GL_DEPTH_COMPONENT,
    GL_UNSIGNED_SHORT,
    GL_NEAREST,
    GL_CLAMP_TO_EDGE
>;

// GL_OES_depth24 extension
using depth24_texture_policy = texture_policy
<
    GL_DEPTH_COMPONENT24_OES,
    GL_DEPTH_COMPONENT,
    GL_UNSIGNED_INT,
    GL_NEAREST,
    GL_CLAMP_TO_EDGE
>;

} /* rendering */
} /* eps */

#endif // RENDERING_TEXTURE_POLICY_H_INCLUDED
