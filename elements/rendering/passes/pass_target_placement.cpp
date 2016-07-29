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

#include "pass_target_placement.h"
#include "rendering/core/texture_policy.h"
#include <queue>
#include <utility>
#include <algorithm>

namespace eps {
namespace rendering {

template<typename _Type, typename _Compare>
using ttl_queue = std::priority_queue<_Type, std::vector<_Type>, _Compare>;

template<typename _Type, typename _Compare>
inline ttl_queue<_Type, _Compare> make_ttl_queue(_Compare compare)
{
    return ttl_queue<_Type, _Compare>(compare);
}

struct ttl_target : private std::pair<size_t, size_t>
{
    using std::pair<size_t, size_t>::pair;

    size_t ttl() const { return first; }
    size_t target() const { return second; }
};

void pass_target_placement::initialize(size_t count)
{
    if(dependencies_.size() != count)
    {
        dependencies_.resize(count);
        slots_.resize(count);
        links_.resize(count);
    }
}

void pass_target_placement::construct(const math::uvec2 & size)
{
    auto queue = make_ttl_queue<ttl_target>([](const auto & l, const auto & r)
    {
        return l.ttl() > r.ttl();
    });

    for(size_t i = 0, end = links_.size(); i < end; ++i)
    {
        if(links_[i].expired())
        {
            auto it = std::find_if(std::rbegin(dependencies_),
                                   std::rend(dependencies_),
                                   [&i](const auto & dependency)
            {
                return std::find(std::begin(dependency),
                                 std::end(dependency), i) != std::end(dependency);
            });

            if(it != std::rend(dependencies_))
            {
                utils::link<pass_target> link;
                if(!queue.empty() && queue.top().ttl() < i)
                {
                    link = links_[queue.top().target()];
                    queue.pop();
                }
                else
                {
                    link = storage_.request_target<default_texture_policy>(size);
                }

                links_[i] = link;
                queue.push({std::distance(std::begin(dependencies_), it.base()), i});
            }
        }
    }
}

void pass_target_placement::register_target(size_t place, utils::unique<pass_target> target)
{
    if(target)
        links_[place] = storage_.register_target(std::move(target));
}

void pass_target_placement::register_dependency(size_t place,
                                                size_t dependency,
                                                const pass_slot & input,
                                                const pass_slot & output)
{
    dependencies_.add(place, dependency, utils::to_int(input));
    slots_.add(place, utils::to_int(output), utils::to_int(input));
}

utils::link<pass_target> pass_target_placement::get_target(size_t place) const
{
    return place != pass_target_dependencies::default_value() ?
                    links_[place] : utils::link<pass_target>();
}

size_t pass_target_placement::get_dependency(size_t place,
                                             const pass_slot & slot) const
{
    return dependencies_.get(place)[utils::to_int(slot)];
}

size_t pass_target_placement::get_dependency_slot(size_t place,
                                                  const pass_slot & slot) const
{
    return slots_.get(place)[utils::to_int(slot)];
}

void pass_target_placement::clear_targets()
{
    storage_.clear();
    std::fill(std::begin(links_), std::end(links_), utils::link<pass_target>());
}

} /* rendering */
} /* eps */
