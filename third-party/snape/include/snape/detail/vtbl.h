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

#ifndef SNAPE_VISITOR_DETAIL_VTBL_H_INCLUDED
#define SNAPE_VISITOR_DETAIL_VTBL_H_INCLUDED

#include "hash.h"

#include <type_traits>
#include <map>
#include <algorithm>

namespace snape {
namespace detail {

    template<typename _From, typename _To>
    using constancy = typename std::conditional<std::is_const<_From>::value, const _To, _To>::type;

    template<typename _Visitor, typename _Base>
    struct vtbl_traits
    {
        using base_type = _Base;
        using visitor_type = _Visitor;
        using function_type = void(visitor_type::*)(base_type &);
    };

    template<typename _Traits>
    struct vtbl
    {
        using base_type = typename _Traits::base_type;
        using function_type = typename _Traits::function_type;
        using visitor_type = typename _Traits::visitor_type;

        template<typename _Specific>
        void put(function_type && function)
        {
            vtbl_[get_hash<base_type, _Specific>()] = function;
        }

        function_type get(const hash_type & hash) const
        {
            auto it = vtbl_.find(hash);
            return it != vtbl_.end() ? it->second : nullptr;
        }

    private:

        std::map<hash_type, function_type> vtbl_;
    };

    template<typename _Traits, typename... _Specifics>
    struct vtbl_builder
    {
        using vtbl_type = vtbl<_Traits>;
        using visitor_type = typename _Traits::visitor_type;
        using base_type = typename _Traits::base_type;

        template<typename... _Targets>
        struct targets {};

        template<typename _Visitor, typename _Specific>
        struct has_visit_method
        {
            template<typename _Class, typename _Param>
            static auto test(_Param * p) -> decltype(std::declval<_Class>().visit(*p),
                                                     std::true_type());

            template<typename, typename>
            static std::false_type test(...);

            using type = decltype(test<_Visitor, _Specific>(nullptr));
            static constexpr const bool value = std::is_same<std::true_type, type>::value;
        };

        vtbl_builder()
        {
            put_thunk(targets<_Specifics...>());
        }

        const auto * get_vtbl() const { return &vtbl_; }

    private:

        template<typename _Specific, typename... _Tail>
        void put_thunk(targets<_Specific, _Tail...>)
        {
            using specific_type = constancy<base_type, _Specific>;
            using is_put = typename has_visit_method<visitor_type, specific_type>::type;
            put_thunk(targets<_Specific, _Tail...>(), is_put());
        }

        template<typename _Specific, typename... _Tail>
        void put_thunk(targets<_Specific, _Tail...>, std::true_type)
        {
            vtbl_.template put<_Specific>(&visitor_type::template thunk<base_type, _Specific>);
            put_thunk(targets<_Tail...>());
        }

        template<typename _Specific, typename... _Tail>
        void put_thunk(targets<_Specific, _Tail...>, std::false_type)
        {
            put_thunk(targets<_Tail...>());
        }

        void put_thunk(targets<>) {}

    private:

        vtbl_type vtbl_;
    };

    template<typename _Traits, typename... _Specifics>
    inline const auto * get_vtbl()
    {
        static vtbl_builder<_Traits, typename std::remove_cv<_Specifics>::type...> builder;
        return builder.get_vtbl();
    }

} /* detail */
} /* snape */

#endif // SNAPE_VISITOR_DETAIL_VTBL_H_INCLUDED
