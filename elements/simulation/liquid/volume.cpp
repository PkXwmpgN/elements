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

#include "volume.h"
#include "math/common.h"

#include <algorithm>
#include <numeric>

namespace eps {
namespace simulation {
namespace liquid {

const short RANGES_COUNT = 3;

bool volume::construct(const math::vec2 & size, size_t particles_count)
{
    if(resize(size, particles_count))
    {
        const size_t bins_count = size_grid_.x * size_grid_.y;

        particles_.resize(particles_count);
        particles_cache_.resize(particles_count);

        counting_.resize(bins_count);
        counting_cache_.resize(bins_count);

        neighbors_.resize(bins_count * (RANGES_COUNT + RANGES_COUNT));

        return true;
    }

    return false;
}

bool volume::resize(const math::vec2 & size, size_t particles_count)
{
    const float distance = 2.0f * std::sqrt((size.x * size.y) / particles_count);
    const math::uvec2 size_grid(math::ceil(size.x / distance),
                                math::ceil(size.y / distance));

    if(size_grid_ != size_grid)
    {
        size_ = size;
        size_grid_ = size_grid;
        distance_ = distance;
        distance2_ = distance_ * distance_;
        return true;
    }

    return false;
}

void volume::partition()
{
    std::fill(counting_.begin(), counting_.end(), 0);

    for(auto & particle : particles_)
    {
        particle.binid = static_cast<size_t>(particle.pos.y / distance_) * size_grid_.x +
                         static_cast<size_t>(particle.pos.x / distance_);

        ++counting_[particle.binid];
    }

    std::partial_sum(counting_.begin(), counting_.end(), counting_.begin());
    std::copy(counting_.begin(), counting_.end(), counting_cache_.begin());

    for(auto & particle : particles_)
    {
        particles_cache_[counting_cache_[particle.binid] - 1] = particle;
        --counting_cache_[particle.binid];
    }

    particles_.swap(particles_cache_);

    fill_neighbors();
}

volume::particles_range volume::get_particles_range()
{
    return utils::make_range(particles_.begin(), particles_.end());
}

volume::particles_range volume::get_neighbors_range(size_t particle_index,
                                                        size_t range_index)
{
    auto start = std::next(neighbors_.begin(), particles_[particle_index].binid *
                           (RANGES_COUNT + RANGES_COUNT) + (2 * range_index));
    return utils::make_range(std::next(particles_.begin(), *start),
                             std::next(particles_.begin(), *(start + 1)));
}

size_t volume::get_neighbors_range_count() const
{
    return RANGES_COUNT;
}

void volume::fill_neighbors()
{
    // TODO: only RANGES_COUNT = 3

    auto it = neighbors_.begin();
    for(size_t y = 0; y < size_grid_.y; ++y)
    {
        for(size_t x = 0; x < size_grid_.x; ++x)
        {
            size_t cx = (x > 0 ? 1 : 0) + (x < (size_grid_.x - 1) ? 1 : 0) + 1;
            size_t cy = (y > 0 ? 1 : 0) + (y < (size_grid_.y - 1) ? 1 : 0) + 1;
            size_t start = ((y > 0) ? y - 1 : y) * size_grid_.x + ((x > 0) ? x - 1 : x);
            size_t zero = 3 - cy;

            while(cy--)
            {
                *it = start ? counting_[start - 1] : 0; ++it;
                *it = counting_[start - 1 + cx]; ++it;

                start += size_grid_.x;
            }

            while(zero--)
            {
                *it = 0; ++it;
                *it = 0; ++it;
            }
        }
    }
}

} /* liquid */
} /* simulation */
} /* eps */
