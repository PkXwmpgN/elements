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

#include "shader.h"
#include <vector>

namespace eps {
namespace rendering {

shader::shader(const char * source, shader_type type)
    : product_(glCreateShader(static_cast<GLenum>(type)))
{
    glShaderSource(utils::raw_product(product_), 1, &source, nullptr);
}

shader::~shader()
{
    glDeleteShader(utils::raw_product(product_));
}

bool shader::compile()
{
    glCompileShader(utils::raw_product(product_));

    GLint status;
    glGetShaderiv(utils::raw_product(product_), GL_COMPILE_STATUS, &status);

    if(status == GL_FALSE)
    {
        GLint length = 0;
        glGetShaderiv(utils::raw_product(product_), GL_INFO_LOG_LENGTH, &length);

        std::string log(length, 0);
        glGetShaderInfoLog(utils::raw_product(product_), length, nullptr, &log.front());

        // TODO: to error log

        return false;
    }
    return true;
}

const product_type & shader::get_product() const
{
    return product_;
}

} /* rendering */
} /* eps */
