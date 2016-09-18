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
// An implementation of a visitor of scene entities.
// Acyclic visitor pattern without RTTI.
//---------------------------------------------------------------

#ifndef STD_DESIGN_VISITOR_H_INCLUDED
#define STD_DESIGN_VISITOR_H_INCLUDED

#include <type_traits>
#include <array>

namespace eps {
namespace design {

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

    template<typename _Base, typename _Function, size_t _Max>
    struct vtbl
    {
        vtbl()
        {
            vtbl_.fill(nullptr);
        }

        template<typename _Specific>
        void add(_Function && function)
        {
            vtbl_[get_tag<_Base, _Specific>()] = std::move(function);
        }

        _Function operator[](size_t index) const
        {
            return vtbl_[index];
        }

    private:

        std::array<_Function, _Max> vtbl_;
    };

    template<typename _Visitor, typename... _Specifics>
    struct vtbl_builder
    {
        using visitor_type = _Visitor;
        using vtbl_type = typename visitor_type::vtbl_type;

        template<typename... _Targets>
        struct targets {};

        vtbl_builder()
        {
            add_thunk(targets<_Specifics...>());
        }

        const vtbl_type * get_vtbl() const { return &vtbl_; }

    private:

        template<typename _Specific, typename... _Tail>
        void add_thunk(targets<_Specific, _Tail...>)
        {
            vtbl_.template add<_Specific>(&visitor_type::template thunk<_Specific>);
            add_thunk(targets<_Tail...>());
        }

        void add_thunk(targets<>) {}

    private:

        vtbl_type vtbl_;
    };

    template<typename _Visitor, typename... _Specifics>
    inline const typename _Visitor::vtbl_type * get_vtbl()
    {
        static vtbl_builder<typename std::remove_cv<_Visitor>::type,
                            typename std::remove_cv<_Specifics>::type...> builder;
        return builder.get_vtbl();
    }

} /* details */

const size_t VISITABLE_HIERARCHY_DEPTH = 16;

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

template<typename _Visitor, typename _Base, typename... _Args>
struct visitor
{
    using vtbl_type = details::vtbl<_Base, void(visitor::*)(_Base &, _Args&&...),
                                    VISITABLE_HIERARCHY_DEPTH>;

    visitor()
        : vtbl_(static_cast<_Visitor*>(this)->get_vtbl())
    {}

    template<typename _Specific>
    void thunk(_Base & base, _Args&& ... args)
    {
        static_cast<_Visitor*>(this)->visit(static_cast<_Specific&>(base),
                                            std::forward<_Args>(args)...);
    }

    void operator()(_Base & base, _Args&& ... args)
    {
        if(auto thunk = (*vtbl_)[base.get_invocation()])
            (this->*thunk)(base, std::forward<_Args>(args)...);
    }

private:

    const vtbl_type * vtbl_;
};

} /* design */
} /* eps */

#define EPS_DESIGN_VISITABLE() \
virtual const size_t & get_invocation() const \
{\
    using this_type = std::remove_cv<typename std::remove_pointer<decltype(this)>::type>::type; \
    return this->get_tag<this_type>(); \
}

#define EPS_DESIGN_VISIT(...) \
const vtbl_type * get_vtbl() const \
{ \
    using this_type = std::remove_cv<typename std::remove_pointer<decltype(this)>::type>::type; \
    return eps::design::details::get_vtbl<this_type, __VA_ARGS__>(); \
};


#endif // STD_DESIGN_VISITOR_H_INCLUDED
