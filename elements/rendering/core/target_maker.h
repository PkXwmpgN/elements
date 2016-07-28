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

#ifndef RENDERING_TARGET_MAKER_H_INCLUDED
#define RENDERING_TARGET_MAKER_H_INCLUDED

#include "target.h"
#include "target_buffered.h"
#include "texture_maker.h"
#include "utils/std/pointer.h"
#include "utils/std/enum.h"
#include <array>

namespace eps {
namespace rendering {

class target_maker
{
public:

    target_maker() = default;
    target_maker(const target_maker &) = delete;
    target_maker & operator=(const target_maker&) = delete;
    target_maker(target_maker &&) = default;
    target_maker & operator=(target_maker &&) = default;

    template<typename _Maker, typename... _Args>
    void register_maker(target::attachment id, _Args&& ...args);

    target construct(const math::uvec2 & size) const;
    target_buffered construct_buffered(const math::uvec2 & size) const;

private:

    std::array
    <
        utils::pointer<texture_maker>,
        utils::to_int(target::attachment::MAX)
    >
    makers_;
};

template<typename _Policy>
inline target_maker get_target_maker()
{
    target_maker maker;
    maker.register_maker<texture_maker_policy<_Policy>>(target::attachment::color0);

    return maker;
}

template<typename _Maker, typename... _Args>
inline void target_maker::register_maker(target::attachment id, _Args&& ...args)
{
    static_assert(std::is_base_of<texture_maker, _Maker>::value, "failed");
    makers_[utils::to_int(id)] = utils::make_shared<_Maker>(std::forward<_Args>(args)...);
}

} /* rendering */
} /* eps */

#endif // RENDERING_TARGET_MAKER_H_INCLUDED
