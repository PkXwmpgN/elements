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

#ifndef RENDERING_TARGET_DOUBLE_BUFFER_H_INCLUDED
#define RENDERING_TARGET_DOUBLE_BUFFER_H_INCLUDED

#include "opengl.h"
#include "target.h"
#include "math/types.h"

namespace eps {
namespace rendering {

template<typename _Attachement_policy>
class target_db
{

public:

    using target_type  = target<_Attachement_policy>;

    target_db();

    bool construct(const math::uvec2 & size);
    void swap();

    const product_type & get_target() const;
    const product_type & get_product() const;
    const math::uvec2 & get_size() const;

private:

    target_type targets_[2];
    enum_type write_;
};

template<typename _Attachement_policy>
inline target_db<_Attachement_policy>::target_db()
    : write_(0)
{}

template<typename _Attachement_policy>
inline bool target_db<_Attachement_policy>::construct(const math::uvec2 & size)
{
    for(auto & target : targets_)
        if(!target.construct(size))
            return false;
    return true;
}

template<typename _Attachement_policy>
inline void target_db<_Attachement_policy>::swap()
{
    write_ = 1 - write_;
}

template<typename _Attachement_policy>
inline const product_type & target_db<_Attachement_policy>::get_target() const
{
    return targets_[write_].get_target();
}

template<typename _Attachement_policy>
inline const product_type & target_db<_Attachement_policy>::get_product() const
{
    return targets_[1 - write_].get_product();
}

template<typename _Attachement_policy>
inline const math::uvec2 & target_db<_Attachement_policy>::get_size() const
{
    return targets_[1 - write_].get_size();
}

} /* rendering */
} /* eps */

#endif // RENDERING_TARGET_DOUBLE_BUFFER_H_INCLUDED
