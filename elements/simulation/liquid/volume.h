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

#ifndef SIMULATION_LIQUID_VOLUME_H_INCLUDED
#define SIMULATION_LIQUID_VOLUME_H_INCLUDED

#include "particle.h"
#include "math/types.h"
#include "utils/std/iterator_range.h"

#include <vector>

namespace eps {
namespace simulation {
namespace liquid {

class volume
{
public:

    using particles_storage = std::vector<particle>;
    using particles_range = utils::iterator_range<typename particles_storage::iterator>;
    using indexes_storage = std::vector<size_t>;

    bool construct(const math::vec2 & size, size_t particles_count);
    void partition();

    particles_range get_particles_range();
    particles_range get_neighbors_range(size_t particle_index, size_t range_index);
    size_t get_neighbors_range_count() const;

    const math::vec2 & get_size() const { return size_; }

    float get_distance() const { return distance_; }
    float get_distance2() const { return distance2_; }

private:

    bool resize(const math::vec2 & size, size_t particles_count);
    void fill_neighbors();

private:

    particles_storage particles_;
    particles_storage particles_cache_;

    indexes_storage counting_;
    indexes_storage counting_cache_;

    indexes_storage neighbors_;

    math::vec2  size_;
    math::uvec2 size_grid_;

    float distance_  = 0.0f;
    float distance2_ = 0.0f;
};

} /* liquid */
} /* simulation */
} /* eps */

#endif // SIMULATION_LIQUID_VOLUME_H_INCLUDED
