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

#ifndef RENDERING_PASSES_PASS_TARGET_STORAGE_H_INCLUDED
#define RENDERING_PASSES_PASS_TARGET_STORAGE_H_INCLUDED

#include "math/types.h"
#include "utils/std/pointer.h"
#include "pass_target.h"

#include <vector>

namespace eps {
namespace rendering {

class pass_target_storage
{
public:

    utils::link<pass_target> register_target(utils::unique<pass_target> target);

    template<typename _Target>
    utils::link<pass_target> request_target(const math::uvec2 & size)
    {
        storage_.push_back(utils::make_shared<_Target>(size));
        return storage_.back();
    }

    void clear() { storage_.clear(); }

private:

    std::vector<utils::pointer<pass_target>> storage_;
};

} /* rendering */
} /* eps */

#endif // RENDERING_PASSES_PASS_TARGET_STORAGE_H_INCLUDED
