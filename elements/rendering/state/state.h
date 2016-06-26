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

#ifndef RENDERING_STATE_STATE_H_INCLUDED
#define RENDERING_STATE_STATE_H_INCLUDED

namespace eps {
namespace rendering {

template<typename _State>
struct state_traits;

template
<
    typename _State,
    typename _State_traits = state_traits<_State>
>
struct state_local
{
    template<typename... _Params>
    explicit state_local(_Params&& ...params)
    {
        _State_traits::init(value_);
        _State_traits::set(std::forward<_Params>(params)...);
    }
    state_local(const state_local &) = delete;
    state_local & operator=(const state_local &) = delete;
    state_local(state_local &&) = default;
    state_local & operator=(state_local &&) = default;

    ~state_local()
    {
        _State_traits::restore(value_);
    }

private:

    typename _State_traits::value_type value_;
};

template<typename _State, typename... _State_params>
state_local<_State> make_state(_State_params&& ...params)
{
    return state_local<_State>(std::forward<_State_params>(params)...);
}

} /* rendering */
} /* eps */

#endif // RENDERING_STATE_STATE_H_INCLUDED
