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

#include "square.h"
#include "rendering/state/state_macro.h"

namespace eps {
namespace rendering {
namespace primitive {

square::square(buffer_usage usage)
    : texture_vertices_(usage)
{
    // elements
    GLubyte elements[] =
    {
        2, 3, 0,
        0, 1, 2
    };
    texture_indicies_.allocate(elements, 6 * sizeof(GLubyte));

    // vertices
    GLfloat vertices[] =
    {
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f
    };

    texture_vertices_.allocate(vertices, 16 * sizeof(GLfloat));
}

void square::render(program & program, short a_position)
{
    EPS_STATE_VERTICES(texture_vertices_.get_product());
    EPS_STATE_INDECIES(texture_indicies_.get_product());

    program.attribute_array(a_position, 0, 2, 4 * sizeof(GLfloat));
    program.attribute_array_enable(a_position);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}

void square::render(program & program, short a_position, short a_texture)
{
    EPS_STATE_VERTICES(texture_vertices_.get_product());
    EPS_STATE_INDECIES(texture_indicies_.get_product());

    program.attribute_array(a_position, 0, 2, 4 * sizeof(GLfloat));
    program.attribute_array_enable(a_position);

    program.attribute_array(a_texture, 2 * sizeof(GLfloat), 2, 4 * sizeof(GLfloat));
    program.attribute_array_enable(a_texture);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}

void square::construct(float * vertices)
{
    texture_vertices_.allocate(vertices, 16 * sizeof(float));
}

} /* primitive */
} /* rendering */
} /* eps */
