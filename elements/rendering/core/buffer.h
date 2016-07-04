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

#ifndef RENDERING_BASE_BUFFER_H_INCLUDED
#define RENDERING_BASE_BUFFER_H_INCLUDED

#include "opengl.h"

namespace eps {
namespace rendering {

enum class buffer_type
{
    VERTEX = GL_ARRAY_BUFFER,
    INDEX  = GL_ELEMENT_ARRAY_BUFFER
};

enum class buffer_usage
{
    STREAM_DRAW  = GL_STREAM_DRAW,
    STATIC_DRAW  = GL_STATIC_DRAW,
    DYNAMIC_DRAW = GL_DYNAMIC_DRAW
};

template<buffer_type _Type>
class buffer
{
public:

    explicit buffer(buffer_usage usage = buffer_usage::STATIC_DRAW);
    ~buffer();

    buffer(buffer &&) = default;
    buffer & operator=(buffer &&) = default;

    void allocate(const void * data, size_t count, size_t size);
    const product_type & get_product() const;
    size_t get_count() const;

private:

    product_type product_;
    buffer_usage usage_;

    size_t count_ = 0;
};

using vertices = buffer<buffer_type::VERTEX>;
using indices = buffer<buffer_type::INDEX>;

template<buffer_type _Type>
inline buffer<_Type>::buffer(buffer_usage usage)
    : usage_(usage)
{
    glGenBuffers(1, utils::ptr_product(product_));
}

template<buffer_type _Type>
inline buffer<_Type>::~buffer()
{
    glDeleteBuffers(1, utils::ptr_product(product_));
}

template<buffer_type _Type>
inline void buffer<_Type>::allocate(const void * data, size_t count, size_t size)
{
    count_ = count;
    glBindBuffer(static_cast<GLenum>(_Type), utils::raw_product(product_));
    glBufferData(static_cast<GLenum>(_Type), count * size, data,
                 static_cast<GLenum>(usage_));
}

template<buffer_type _Type>
inline const product_type & buffer<_Type>::get_product() const
{
    return product_;
}

template<buffer_type _Type>
inline size_t buffer<_Type>::get_count() const
{
    return count_;
}

} /* rendering */
} /* eps */

#endif // RENDERING_BASE_BUFFER_H_INCLUDED
