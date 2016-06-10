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

#include "framerate.h"
#include <chrono>

namespace eps {
namespace timing {

framerate::framerate(unsigned int rate)
    : rate_(rate)
    , counter_(0)
    , last_(0)
    , elapsed_(0)
    , frame_(0)
    , fps_(rate)
{}

bool framerate::update()
{
    using namespace std::chrono;

    const auto now = steady_clock::now().time_since_epoch();

    auto current = duration_cast<milliseconds>(now).count();
    auto desired_fps = 1000 / rate_;

    elapsed_ += last_ ? current - last_ : 0;
    last_ = current;

    if(current - frame_ > 1000)
    {
        fps_ = frame_ ? counter_ / (float(current - frame_) / 1000.0f) : 0.0f;
        counter_ = 0;
        frame_ = current;
    }
    ++counter_;

    if(elapsed_ > desired_fps)
    {
        elapsed_ -= desired_fps;
        return true;
    }

    return false;
}

} /* timing */
} /* eps */
