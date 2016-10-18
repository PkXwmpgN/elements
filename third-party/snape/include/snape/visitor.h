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

#include <type_traits>
#include <array>

namespace snape {
namespace details {

    template<typename _Base>
    struct tag_generator
    {
        static size_t counter;
    };

    template<typename _Base>
    size_t tag_generator<_Base>::counter = 0;

    template<typename _Base, typename _Specific>
    struct tag
    {
        static size_t value;
    };

    template<typename _Base, typename _Specific>
    size_t tag<_Base, _Specific>::value = tag_generator<_Base>::counter++;

    template<typename _Base, typename _Specific>
    inline const size_t & get_tag()
    {
        return tag<typename std::remove_cv<_Base>::type,
                   typename std::remove_cv<_Specific>::type>::value;
    }


    template<typename _From, typename _To>
    using constancy = typename std::conditional<std::is_const<_From>::value, const _To, _To>::type;

    template<typename _Visitor, typename _Base, size_t _Depth>
    struct vtbl_traits
    {
        using base_type = _Base;
        using visitor_type = _Visitor;
        using function_type = void(visitor_type::*)(base_type &);

        static constexpr size_t get_depth() { return _Depth; }
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
            vtbl_[get_tag<base_type, _Specific>()] = function;
        }

        function_type get(size_t index) const
        {
            return vtbl_[index];
        }

    private:

        std::array<function_type, _Traits::get_depth()> vtbl_ = { {nullptr} };
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
            static_assert(has_visit_method<visitor_type, specific_type>::value,
                          "Failed: require 'visit' method");

            vtbl_.template put<_Specific>(&visitor_type::template thunk<base_type, _Specific>);
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

} /* details */


template <typename _Base>
struct visitable
{

protected:

    template<typename _Specific>
    const size_t & get_tag() const
    {
        return details::get_tag<_Base, _Specific>();
    }
};

template<typename _Base>
struct visitor_traits
{
    using base_type = _Base;
    static constexpr size_t get_depth() { return 16; }
};

template<typename _Visitor, typename _Traits>
struct visitor
{
    using visitor_type = _Visitor;
    using traits_type  = _Traits;

    template<typename _Base, typename _Specific>
    void thunk(_Base & base)
    {
        using specific_type = details::constancy<_Base, _Specific>;
        static_cast<visitor_type*>(this)->visit(static_cast<specific_type&>(base));
    }

    template<typename _Base>
    void operator()(_Base & base)
    {
        static_assert(std::is_base_of<typename traits_type::base_type, _Base>::value,
                      "Failed: '_Base' type must be derived from '_Traits::base_type' type");

        using base_type = details::constancy<_Base, typename traits_type::base_type>;
        using traits_type = details::vtbl_traits<visitor_type, base_type, _Traits::get_depth()>;

        const auto * vtbl = static_cast<visitor_type*>(this)->template get_vtbl<traits_type>();
        if(auto thunk = vtbl->get(base.get_invocation()))
            (static_cast<visitor_type*>(this)->*thunk)(base);
    }
};

} /* snape */

#define SNAPE_VISITABLE() \
virtual const size_t & get_invocation() const \
{\
    using this_type = std::remove_cv<typename std::remove_pointer<decltype(this)>::type>::type; \
    return get_tag<this_type>(); \
}

#define SNAPE_VISIT(...) \
template<typename _Traits> \
const auto * get_vtbl() const \
{ \
    return snape::details::get_vtbl<_Traits, __VA_ARGS__>(); \
};

#endif // SNAPE_VISITOR_H_INCLUDED
