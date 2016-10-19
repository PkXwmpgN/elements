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

//---------------------------------------------------------------
// Acyclic visitor pattern without RTTI.
//---------------------------------------------------------------

#ifndef SNAPE_VISITOR_H_INCLUDED
#define SNAPE_VISITOR_H_INCLUDED

#include "detail/hash.h"
#include "detail/vtbl.h"

namespace snape {

template <typename _Base>
struct visitable
{
    using base_type = _Base;
};

template<typename _Base>
struct visitor_traits
{
    using base_type = _Base;
};

template<typename _Visitor, typename _Traits>
struct visitor
{
    using visitor_type = _Visitor;
    using traits_type  = _Traits;

    template<typename _Base, typename _Specific>
    void thunk(_Base & base)
    {
        using specific_type = detail::constancy<_Base, _Specific>;
        static_cast<visitor_type*>(this)->visit(static_cast<specific_type&>(base));
    }

    template<typename _Base>
    void operator()(_Base & base)
    {
        static_assert(std::is_base_of<typename traits_type::base_type, _Base>::value, "");

        using base_type = detail::constancy<_Base, typename traits_type::base_type>;
        using traits_type = detail::vtbl_traits<visitor_type, base_type>;

        const auto * vtbl = static_cast<visitor_type*>(this)->template get_vtbl<traits_type>();
        if(auto thunk = vtbl->get(base.get_hash()))
            (static_cast<visitor_type*>(this)->*thunk)(base);
    }
};

} /* snape */

#define SNAPE_VISITABLE(Specific) \
static constexpr snape::detail::hash_type hash__ = snape::detail::get_hash<base_type, Specific>(); \
virtual snape::detail::hash_type get_hash() const \
{\
    return hash__; \
}

#define SNAPE_VISIT(...) \
template<typename _Traits> \
const auto * get_vtbl() const \
{ \
    return snape::detail::get_vtbl<_Traits, __VA_ARGS__>(); \
};

#endif // SNAPE_VISITOR_H_INCLUDED
