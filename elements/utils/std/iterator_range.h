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

#ifndef STD_UTILS_ITERATOR_RANGE_H_INCLUDED
#define STD_UTILS_ITERATOR_RANGE_H_INCLUDED

#include <iterator>
#include <utility>

namespace eps {
namespace utils {

template<typename _Iterator>
struct iterator_range : std::pair<_Iterator, _Iterator>
{
    using std::pair<_Iterator, _Iterator>::pair;

    auto begin() const { return this->first; }
    auto end() const { return this->second; }

    bool empty() const { return this->first == this->second; }
    auto size() const { return std::distance(this->first, this->second); }
};

template<typename _Iterator>
inline auto begin(const iterator_range<_Iterator> & r)
{
    return r.begin();
}

template<typename _Iterator>
inline auto end(const iterator_range<_Iterator> & r)
{
    return r.end();
}

template<typename _Iterator>
inline iterator_range<_Iterator> make_range(_Iterator begin, _Iterator end)
{
    return iterator_range<_Iterator>(begin, end);
}

} /* utils */
} /* eps */

#endif // STD_UTILS_ITERATOR_RANGE_H_INCLUDED
