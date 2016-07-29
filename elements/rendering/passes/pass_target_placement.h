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

#ifndef RENDERING_PASSES_PASS_TARGET_PLACEMENT_H_INCLUDED
#define RENDERING_PASSES_PASS_TARGET_PLACEMENT_H_INCLUDED

#include "math/types.h"

#include "utils/std/dependency_index.h"
#include "utils/std/enum.h"

#include "pass_slot.h"
#include "pass_target.h"
#include "pass_target_storage.h"

#include <vector>

namespace eps {
namespace rendering {

class pass_target_placement
{
public:

    void initialize(size_t place_count);
    void construct(const math::uvec2 & size);

    void register_target(size_t place, utils::unique<pass_target> target);
    void register_dependency(size_t place, size_t dependency,
                             const pass_slot & input, const pass_slot & output);

    utils::link<pass_target> get_target(size_t place) const;

    size_t get_dependency(size_t place, const pass_slot & input) const;
    size_t get_dependency_slot(size_t place, const pass_slot & input) const;

    void clear_targets();

private:

    using pass_target_links = std::vector<utils::link<pass_target>>;
    using pass_target_dependencies = utils::dependency_index<utils::to_int(pass_slot::MAX), size_t, size_t(-1)>;
    using pass_target_slots = utils::dependency_index<utils::to_int(pass_slot::MAX), size_t, 0>;

    pass_target_storage storage_;
    pass_target_links links_;
    pass_target_dependencies dependencies_;
    pass_target_slots slots_;
};

} /* rendering */
} /* eps */

#endif // RENDERING_PASSES_PASS_TARGET_PLACEMENT_H_INCLUDED
