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

#ifndef PLATFORM_ANDROID_LIQUID_RENDERER_FACTORY_H_INCLUDED
#define PLATFORM_ANDROID_LIQUID_RENDERER_FACTORY_H_INCLUDED

#include <memory>
#include <vector>
#include <algorithm>

template<typename _TRenderer>
class renderer_factory
{
public:

    using renderer_type = _TRenderer;
    using renderer_pointer = std::shared_ptr<renderer_type>;

    template<typename ..._Tparams>
    size_t open(_Tparams... params)
    {
        auto closed = std::find(renderers_.begin(), renderers_.end(), nullptr);
        if(closed == renderers_.end())
        {
            renderers_.emplace_back(new renderer_type(params...));
            return renderers_.size() - 1;
        }

        closed->reset(new renderer_type(params...));
        return std::distance(renderers_.begin(), closed);
    }

    bool close(size_t id)
    {
        if(id >= renderers_.size())
            return false;

        renderers_[id] = nullptr;
        return true;
    }

    renderer_pointer get(size_t id)
    {
        return (id < renderers_.size()) ? renderers_[id] : nullptr;
    }

private:

    std::vector<renderer_pointer> renderers_;
};

#endif // PLATFORM_ANDROID_LIQUID_RENDERER_FACTORY_H_INCLUDED
