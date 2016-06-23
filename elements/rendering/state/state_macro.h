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

#ifndef RENDERING_STATE_STATE_MACRO_H_INCLUDED
#define RENDERING_STATE_STATE_MACRO_H_INCLUDED

#include "state.h"
#include "state_traits.h"

#define __STATE_CONCATENATE_NAME_IMPL__(x, y) x##y
#define __STATE_CONCATENATE_NAME__(x, y) __STATE_CONCATENATE_NAME_IMPL__(x, y)
#define __STATE_MAKE_NAME__(x) __STATE_CONCATENATE_NAME__(x, __LINE__)

#define __STATE_BASE_DETAIL__(name, stored_state, ...) \
    auto name = eps::rendering::make_state<stored_state>(__VA_ARGS__);
#define __STATE_BASE__(stored_state, ...) \
    __STATE_BASE_DETAIL__(__STATE_MAKE_NAME__(_state_), stored_state, __VA_ARGS__)

#define EPS_STATE_VIEWPORT(...) __STATE_BASE__(eps::rendering::state_viewport, __VA_ARGS__);
#define EPS_STATE_BLEND(...) __STATE_BASE__(eps::rendering::state_blend, __VA_ARGS__);
#define EPS_STATE_TARGET(...) __STATE_BASE__(eps::rendering::state_target, __VA_ARGS__);
#define EPS_STATE_SAMPLER(...) __STATE_BASE__(eps::rendering::state_sampler, __VA_ARGS__);
#define EPS_STATE_SAMPLER_0(...) __STATE_BASE__(eps::rendering::state_sampler, __VA_ARGS__, 0);
#define EPS_STATE_SAMPLER_1(...) __STATE_BASE__(eps::rendering::state_sampler, __VA_ARGS__, 1);
#define EPS_STATE_SAMPLER_2(...) __STATE_BASE__(eps::rendering::state_sampler, __VA_ARGS__, 2);
#define EPS_STATE_SAMPLER_3(...) __STATE_BASE__(eps::rendering::state_sampler, __VA_ARGS__, 3);
#define EPS_STATE_VERTICES(...) __STATE_BASE__(eps::rendering::state_vertices, __VA_ARGS__);
#define EPS_STATE_INDICES(...) __STATE_BASE__(eps::rendering::state_indices, __VA_ARGS__);
#define EPS_STATE_PROGRAM(...) __STATE_BASE__(eps::rendering::state_program, __VA_ARGS__);


#endif // RENDERING_STATE_STATE_MACRO_H_INCLUDED
