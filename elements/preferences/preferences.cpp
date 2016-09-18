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

#include "preferences.h"
#include <cassert>

namespace eps {

std::unique_ptr<preferences> preferences::instance_ = nullptr;

preferences::preferences(preferences_interface * impl)
    : impl_(impl)
{}

preferences & preferences::instance()
{
    assert(instance_ != nullptr);
    return *instance_;
}

void preferences::register_listener(on_changed_listener * listener)
{
    listeners_.insert(listener);
}

void preferences::unregister_listener(on_changed_listener * listener)
{
    listeners_.erase(listener);
}

void preferences::on_changed(const std::string & key, float value)
{
    for(auto & listener : listeners_)
        listener->on_changed(key, value);
}

} /* namespace eps */
