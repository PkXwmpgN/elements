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

#ifndef UTILS_STD_DEPENDENCY_INDEX_H_INCLUDED
#define UTILS_STD_DEPENDENCY_INDEX_H_INCLUDED

#include <vector>
#include <array>
#include <set>
#include <algorithm>

namespace eps {
namespace utils {

template<size_t _Dependency_count, typename _Type, _Type _Default,
         typename _Enabled = typename std::enable_if<std::is_integral<_Type>::value>::type>
class dependency_index
{
public:

    using value_type = _Type;
    using dependency_array = std::array<_Type, _Dependency_count>;
    using dependency_list = std::vector<dependency_array>;

    static value_type default_value() { return _Default; }

    void resize(size_t elements_count)
    {
        dependencies_.resize(elements_count);
        for(auto & elem : dependencies_)
            std::fill(std::begin(elem), std::end(elem), _Default);
    }

    void add(value_type target, value_type dependency,
             size_t dependency_index)
    {
        dependencies_[target][dependency_index] = dependency;
    }

    const dependency_array & get(value_type target) const
    {
        return dependencies_[target];
    }

    auto begin() const { return dependencies_.begin(); }
    auto end() const { return dependencies_.end(); }
    auto rbegin() const { return dependencies_.rbegin(); }
    auto rend() const { return dependencies_.rend(); }

    size_t size() const { return dependencies_.size(); }

private:

    dependency_list dependencies_;
};

} /* utils */
} /* eps */

#endif // UTILS_STD_DEPENDENCY_INDEX_H_INCLUDED
