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

#ifndef STD_UTILS_VALUES_H_INCLUDED
#define STD_UTILS_VALUES_H_INCLUDED

#include <functional>
#include <initializer_list>

namespace eps {
namespace utils {

template<typename _Type, typename _Equal = std::equal_to<_Type>>
struct values
{
    explicit values(std::initializer_list<_Type> v)
        : values_(v)
    {}

    friend bool operator==(const _Type & l, const values & r) { return r.equal(l); }
    friend bool operator==(const values & l, const _Type & r) { return l.equal(r); }
    friend bool operator!=(const _Type & l, const values & r) { return !r.equal(l); }
    friend bool operator!=(const values & l, const _Type & r) { return !l.equal(r); }

private:

    bool equal(const _Type & obj) const
    {
        _Equal opt;
        for(const auto & v : values_)
            if(opt(v, obj))
                return true;
        return false;
    }

private:

    std::initializer_list<_Type> values_;
};

} /* utils */
} /* eps */

#endif // STD_UTILS_VALUES_H_INCLUDED
