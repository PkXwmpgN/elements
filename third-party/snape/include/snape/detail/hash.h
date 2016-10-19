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

#ifndef SNAPE_VISITOR_DETAIL_HASH_H_INCLUDED
#define SNAPE_VISITOR_DETAIL_HASH_H_INCLUDED

#include <cstdint>
#include <ctti/type_id.hpp>

namespace snape {
namespace detail {

    using hash_type = std::uint64_t;

    template<typename _Base, typename _Specific>
    struct tag {};

    template<typename _Base, typename _Specific>
    inline constexpr hash_type get_hash()
    {
        using tag_type = tag<typename std::remove_cv<_Base>::type,
                             typename std::remove_cv<_Specific>::type>;
        return ctti::unnamed_type_id<tag_type>().hash();
    }

} /* detail */
} /* snape */

#endif // SNAPE_VISITOR_DETAIL_HASH_H_INCLUDED
