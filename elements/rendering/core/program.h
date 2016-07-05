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

#ifndef RENDERING_BASE_PROGRAM_H_INCLUDED
#define RENDERING_BASE_PROGRAM_H_INCLUDED

#include <string>
#include <array>
#include <initializer_list>

#include "opengl.h"
#include "math/types.h"

namespace eps {
namespace rendering {

class shader;
class program
{
    static const short MAX_ATTRIBUTES = 8;
    static const short MAX_UNIFORMS   = 128;

    struct location_data
    {
        const char * name;
        short id;
    };

    using initializer_locations = std::initializer_list<location_data>;

public:

    program();
    ~program();

    program(const program &) = delete;
    program & operator=(const program &) = delete;
    program(program &&) = default;
    program & operator=(program &&) = default;

    bool attach(const shader & object);
    bool link();

    const product_type & get_product() const;

    bool attribute_location(const char * attribute, short attribute_index);
    bool attribute_location(const initializer_locations & attributes);

    void attribute_array(short attribute_index, int offset, int size, int stride);
    void attribute_array_enable(short attrubute_index);
    void attribute_array_disable(short attribute_index);

    bool uniform_location(const char * uniform, short uniform_index);
    bool uniform_location(const initializer_locations & uniforms);

    void uniform_value(short uniform_index, int value);
    void uniform_value(short uniform_index, float value);
    void uniform_value(short uniform_index, float * values, int count);
    void uniform_value(short uniform_index, const math::vec2 & vec2);
    void uniform_value(short uniform_index, const math::vec3 & vec3);
    void uniform_value(short uniform_index, const math::vec4 & vec4);
    void uniform_value(short uniform_index, const math::mat4 & mat4);

private:

    std::array<short, MAX_ATTRIBUTES> attributes_;
    std::array<short, MAX_UNIFORMS>   uniforms_;

    product_type product_;
};

} /* rendering */
} /* eps */

#endif // RENDERING_BASE_PROGRAM_H_INCLUDED
