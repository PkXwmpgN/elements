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

#include "program.h"
#include "shader.h"

namespace eps {
namespace rendering {

const short BAD_LOCATION = -1;

program::program()
    : product_(glCreateProgram())
{
    attributes_.fill(BAD_LOCATION);
    uniforms_.fill(BAD_LOCATION);
}

program::~program()
{
    if(!product_.invalid())
        glDeleteProgram(utils::raw_product(product_));
}

bool program::attach(const shader & object)
{
    glAttachShader(utils::raw_product(product_),
                   utils::raw_product(object.get_product()));
    return true;
}

bool program::link()
{
    glLinkProgram(utils::raw_product(product_));

    GLint status;
    glGetProgramiv(utils::raw_product(product_), GL_LINK_STATUS, &status);

    if(status == GL_FALSE)
    {
        GLint length = 0;
        glGetProgramiv(utils::raw_product(product_), GL_INFO_LOG_LENGTH, &length);

        std::string log(length, 0);
        glGetProgramInfoLog(utils::raw_product(product_), length, nullptr, &log.front());

        // TODO: to error log

        return false;
    }
    return true;
}

const product_type & program::get_product() const
{
    return product_;
}

bool program::attribute_location(const char * attribute, short attribute_index)
{
    if(0 <= attribute_index && attribute_index < MAX_ATTRIBUTES)
    {
        GLint location = glGetAttribLocation(utils::raw_product(product_), attribute);
        if(location != BAD_LOCATION)
        {
            attributes_[attribute_index] = location;
            return true;
        }
    }
    return false;
}

bool program::attribute_location(const initializer_locations & attributes)
{
    for(auto & attr : attributes)
        attribute_location(attr.name, attr.id);
    return true;
}

void program::attribute_array(short attribute_index, int offset, int size, int stride)
{
    const short location = attributes_[attribute_index];
    if(location != BAD_LOCATION)
        glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, stride,
                              reinterpret_cast<const void*>(offset));
}

void program::attribute_array_enable(short attribute_index)
{
    const short location = attributes_[attribute_index];
    if(location != BAD_LOCATION)
        glEnableVertexAttribArray(location);
}

void program::attribute_array_disable(short attribute_index)
{
    short location = attributes_[attribute_index];
    if(location != BAD_LOCATION)
        glDisableVertexAttribArray(location);
}

bool program::uniform_location(const char * uniform, short uniform_index)
{
    if(0 <= uniform_index && uniform_index < MAX_ATTRIBUTES)
    {
        GLint location = glGetUniformLocation(utils::raw_product(product_), uniform);
        if(location != BAD_LOCATION)
        {
            uniforms_[uniform_index] = location;
            return true;
        }
    }
    return false;
}

bool program::uniform_location(const initializer_locations & uniforms)
{
    for(auto & uniform : uniforms)
        uniform_location(uniform.name, uniform.id);
    return true;
}

void program::uniform_value(short uniform_index, int value)
{
    short location = uniforms_[uniform_index];
    if(location != BAD_LOCATION)
        glUniform1i(location, value);
}

void program::uniform_value(short uniform_index, float value)
{
    const short location = uniforms_[uniform_index];
    if(location != BAD_LOCATION)
        glUniform1f(location, value);
}

void program::uniform_value(short uniform_index, float *values, int count)
{
    const short location = uniforms_[uniform_index];
    if(location != BAD_LOCATION)
        glUniform1fv(location, count, values);
}

void program::uniform_value(short uniform_index, const math::vec2 & vec2)
{
    const short location = uniforms_[uniform_index];
    if(location != BAD_LOCATION)
        glUniform2fv(location, 1, math::ptr(vec2));
}

void program::uniform_value(short uniform_index, const math::vec3 & vec3)
{
    const short location = uniforms_[uniform_index];
    if(location != BAD_LOCATION)
        glUniform3fv(location, 1, math::ptr(vec3));
}

void program::uniform_value(short uniform_index, const math::vec4 & vec4)
{
    const short location = uniforms_[uniform_index];
    if(location != BAD_LOCATION)
        glUniform4fv(location, 1, math::ptr(vec4));
}

void program::uniform_value(short uniform_index, const math::mat4 & mat4)
{
    const short location = uniforms_[uniform_index];
    if(location != BAD_LOCATION)
        glUniformMatrix4fv(location, 1, GL_FALSE, math::ptr(mat4));
}

} /* rendering */
} /* eps */
