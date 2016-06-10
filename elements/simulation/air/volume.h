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

#ifndef SIMULATION_AIR_VOLUME_H_INCLUDED
#define SIMULATION_AIR_VOLUME_H_INCLUDED

#include "math/types.h"
#include <vector>

namespace eps {
namespace simulation {
namespace air {

class volume
{
public:

    struct cell
    {
        enum DATA_ID { U, V, U0, V0 };
        float data[4];
    };

    using cells_storage = std::vector<cell>;

public:

    volume();

    bool construct(const math::uvec2 & size);

    void velocity_step(float dt);
    void velocity(float x, float y, float u, float v);

    auto begin() const { return cells_.begin(); }
    auto end() const { return cells_.end(); }

private:

    void hbound(short id, bool revert);
    void vbound(short id, bool revert);
    void project(short u, short v, short p, short d);
    void diffuse(short id, short id0, bool vertical);
    void advect(short d, short d0, short u, short v, bool vertical, float dt);

private:

    cells_storage cells_;

    math::uvec2 size_;
    math::uvec2 size_grid_;
    math::uvec2 size_cell_;
};

} /* air */
} /* simulation */
} /* eps */

#endif // SIMULATION_AIR_VOLUME_H_INCLUDED
