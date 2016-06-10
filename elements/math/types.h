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

#ifndef MATH_TYPES_H_INCLUDED
#define MATH_TYPES_H_INCLUDED

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace eps {
namespace math {

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

using uvec2 = glm::tvec2<size_t, glm::highp>;
using uvec3 = glm::tvec3<size_t, glm::highp>;
using uvec4 = glm::tvec4<size_t, glm::highp>;

using ivec2 = glm::tvec2<int, glm::highp>;
using ivec3 = glm::tvec3<int, glm::highp>;
using ivec4 = glm::tvec4<int, glm::highp>;

using mat2 = glm::mat2;
using mat3 = glm::mat3;
using mat4 = glm::mat4;

template<typename _Type>
inline const typename _Type::value_type * ptr(const _Type & value)
{
    return glm::value_ptr(value);
}

template<typename _Type>
inline typename _Type::value_type * ptr(_Type & value)
{
    return glm::value_ptr(value);
}

} /* math */
} /* eps */

#endif // MATH_H_INCLUDED
