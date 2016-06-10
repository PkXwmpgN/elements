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

#include <stdlib.h>
#include "volume.h"
#include "math/common.h"

#define FOR_BEGIN(size) {\
    size_t row = size.x, index = 0;\
    for(size_t j = 1; j < size.y - 1; ++j, row += size.x) {\
        for(size_t i = 1; i < size.x - 1; ++i) {\
            index = row + i;
#define FOR_END() } } }

namespace eps {
namespace simulation {
namespace air {

volume::volume()
    : size_cell_(16, 16)
{}

bool volume::construct(const math::uvec2 & size)
{
    math::uvec2 size_grid = size / size_cell_;
    if(size_grid_ != size_grid)
    {
        size_ = size;
        size_grid_ = size_grid;

        cells_.resize(size_grid_.x * size_grid_.y);
        std::memset(cells_.data(), cells_.size() * sizeof(cell), 0);

        return true;
    }
    return false;
}

void volume::velocity(float x, float y, float u, float v)
{
    const size_t index = size_t(x / size_cell_.x) +
                         size_t(y / size_cell_.y - 1) * size_grid_.x;
    if(index < cells_.size())
    {
        cells_[index].data[cell::U] += u;
        cells_[index].data[cell::V] += v;
    }
}

void volume::velocity_step(float dt)
{
    diffuse(cell::U0, cell::U, true);
    diffuse(cell::V0, cell::V, false);
    project(cell::U0, cell::V0, cell::U, cell::V);
    advect(cell::U, cell::U0, cell::U0, cell::V0, true, dt);
    advect(cell::V, cell::V0, cell::U0, cell::V0, false, dt);
    project(cell::U, cell::V, cell::U0, cell::V0);
}

void volume::hbound(short id, bool revert)
{
    const size_t tb = 1;
    const size_t te = size_grid_.x - 1;
    const size_t bb = size_grid_.x * (size_grid_.y - 1) + 1;
    const size_t be = size_grid_.x * (size_grid_.y) - 1;

    if(revert)
    {
        for(size_t i = tb; i < te; ++i)
            cells_[i].data[id] = -cells_[i + size_grid_.x].data[id];
        for(size_t i = bb; i < be; ++i)
            cells_[i].data[id] = -cells_[i - size_grid_.x].data[id];
    }
    else
    {
        for(size_t i = tb; i < te; ++i)
            cells_[i].data[id] = cells_[i + size_grid_.x].data[id];
        for(size_t i = bb; i < be; ++i)
            cells_[i].data[id] = cells_[i - size_grid_.x].data[id];
    }

    cells_[tb - 1].data[id] = 0.5f * (cells_[tb].data[id] +
                                      cells_[tb - 1 + size_grid_.x].data[id]);
    cells_[bb - 1].data[id] = 0.5f * (cells_[bb].data[id] +
                                      cells_[bb - 1 - size_grid_.x].data[id]);
}

void volume::vbound(short id, bool revert)
{
    const size_t lb = size_grid_.x;
    const size_t le = size_grid_.x * (size_grid_.y - 1);
    const size_t rb = size_grid_.x + size_grid_.x - 1;
    const size_t re = size_grid_.x * (size_grid_.y - 1) - 1;

    if(revert)
    {
        for(size_t i = lb; i < le; i += size_grid_.x)
            cells_[i].data[id] = -cells_[i + 1].data[id];
        for(size_t i = rb; i < re; i += size_grid_.x)
            cells_[i].data[id] = -cells_[i - 1].data[id];
    }
    else
    {
        for(size_t i = lb; i < le; i += size_grid_.x)
            cells_[i].data[id] = cells_[i + 1].data[id];
        for(size_t i = rb; i < re; i += size_grid_.x)
            cells_[i].data[id] = cells_[i - 1].data[id];
    }

    cells_[rb - size_grid_.x].data[id] = 0.5f * (cells_[rb].data[id] +
                                                 cells_[rb - size_grid_.x - 1].data[id]);
    cells_[re + size_grid_.x].data[id] = 0.5f * (cells_[re].data[id] +
                                                 cells_[re + size_grid_.x - 1].data[id]);
}

void volume::diffuse(short x, short x0, bool vertical)
{
    FOR_BEGIN(size_grid_)
        cells_[index].data[x] = cells_[index].data[x0];
    FOR_END();

    vbound(x, vertical); hbound(x, !vertical);
}

void volume::project(short u, short v, short p, short d)
{
    const float h = 1.0f / (size_grid_.x - 2);

    FOR_BEGIN(size_grid_)
        cells_[index].data[d] = -0.5f * h * (cells_[index + 1].data[u] -
                                             cells_[index - 1].data[u] +
                                             cells_[index + size_grid_.x].data[v] -
                                             cells_[index - size_grid_.x].data[v]);
        cells_[index].data[p] = 0.0f;
    FOR_END();

    vbound(d, false); hbound(d, false);
    vbound(p, false); hbound(p, false);

    const size_t force = 20;
    for(size_t f = 0; f < force; ++f)
    {
        FOR_BEGIN(size_grid_)
            cells_[index].data[p] = (cells_[index].data[d] +
                                     cells_[index - 1].data[p] +
                                     cells_[index + 1].data[p] +
                                     cells_[index - size_grid_.x].data[p] +
                                     cells_[index + size_grid_.x].data[p]) * 0.25f;
        FOR_END();
        vbound(p, false); hbound(p, false);
    }

    FOR_BEGIN(size_grid_)
        cells_[index].data[u] -= 0.5f * (size_grid_.x - 2) *
                (cells_[index + 1].data[p] - cells_[index - 1].data[p]);
    FOR_END();
    vbound(u, true); hbound(u, false);

    FOR_BEGIN(size_grid_)
        cells_[index].data[v] -= 0.5f * (size_grid_.x - 2) *
                (cells_[index + size_grid_.x].data[p] - cells_[index - size_grid_.x].data[p]);
    FOR_END();
    vbound(v, false); hbound(v, true);
}

void volume::advect(short d, short d0, short u, short v, bool vertical, float dt)
{
    float x = 0.0f;
    float y = 0.0f;
    float s0, t0, s1, t1;
    size_t i0, i1, j0, j1;

    const float dtu = dt * (size_grid_.x - 2);
    const float dtv = dt * (size_grid_.y - 2);

    FOR_BEGIN(size_grid_)
        x = math::clamp(float(i) - dtu * cells_[index].data[u], 0.5f, (size_grid_.x - 2) + 0.5f);
        y = math::clamp(float(j) - dtv * cells_[index].data[v], 0.5f, (size_grid_.y - 2) + 0.5f);
        s1 = math::fract(x); s0 = 1.0f - s1;
        t1 = math::fract(y); t0 = 1.0f - t1;
        i0 = (int)x; i1 = i0 + 1;
        j0 = (int)y; j1 = j0 + 1;
        cells_[index].data[d] = s0 * (t0 * cells_[i0 + j0 * size_grid_.x].data[d0] +
                                      t1 * cells_[i0 + j1 * size_grid_.x].data[d0]) +
                                s1 * (t0 * cells_[i1 + j0 * size_grid_.x].data[d0] +
                                      t1 * cells_[i1 + j1 * size_grid_.x].data[d0]);
    FOR_END();

    vbound(d, vertical); hbound(d, !vertical);
}

} /* air */
} /* simulation */
} /* eps */
