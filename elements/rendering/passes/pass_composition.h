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

#ifndef RENDERING_PASSES_PASS_COMPOSITION_H_INCLUDED
#define RENDERING_PASSES_PASS_COMPOSITION_H_INCLUDED

#include "pass_target_input.h"
#include <vector>

namespace eps {
namespace rendering {

class pass_base;

class pass_composition
{
public:

    pass_composition();

    template<typename _Pass>
    utils::link<_Pass> add_pass()
    {
        pass_target_input inputs(passes_.size(), placement_);
        return add_pass<_Pass>(inputs);
    }

    template<typename _Pass>
    utils::link<_Pass> add_pass(const pass_target_input & inputs)
    {
        auto pass = utils::make_shared<_Pass>(passes_.size(), inputs);
        if(!pass->initialize())
            return utils::link<_Pass>();

        passes_.push_back(pass);
        return pass;
    }

    template<typename _Pass_target, typename _Pass_dependency>
    void add_dependency(const utils::link<_Pass_target> & target,
                        const utils::link<_Pass_dependency> & dependency,
                        const pass_slot & input, const pass_slot & output)
    {
        if(!target.expired() && !dependency.expired())
            placement_->register_dependency(target.lock()->get_place(),
                                            dependency.lock()->get_place(),
                                            input, output);
    }

    template<typename _Pass_target, typename _Pass_dependency>
    void add_dependency(const utils::link<_Pass_target> & target,
                        const utils::link<_Pass_dependency> & dependency,
                        const pass_slot & input)
    {
        add_dependency(target, dependency, input, pass_slot::slot_0);
    }

    void initialize(size_t places_count);
    void construct(const math::uvec2 & size);
    void process(float dt);

private:

    std::vector<utils::pointer<pass_base>> passes_;
    utils::pointer<pass_target_placement> placement_;
};


} /* rendering */
} /* eps */

#endif // RENDERING_PASSES_PASS_COMPOSITION_H_INCLUDED
