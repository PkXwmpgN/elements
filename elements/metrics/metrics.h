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

#ifndef METRICS_METRICS_H_INCLUDED
#define METRICS_METRICS_H_INCLUDED

#include "utils/std/pointer.h"

namespace eps {

struct metrics_interface
{
    virtual float density() const = 0;
    virtual ~metrics_interface() {}
};

class metrics
{
public:

    template<typename _TMetrics, typename ..._TArgs>
    static void init(_TArgs&&... args)
    {
        _TMetrics * impl = new _TMetrics(std::forward<_TArgs>(args)...);
        instance_.reset(new metrics(impl));
    }

    static metrics & instance();

    float density() const
    {
        return impl_->density();
    }

private:

    explicit metrics(metrics_interface * impl);

    metrics(const metrics&) = delete;
    metrics(metrics&&) = delete;
    metrics & operator=(const metrics&) = delete;
    metrics & operator=(metrics&&) = delete;

private:

    static utils::unique<metrics> instance_;
    utils::unique<metrics_interface>  impl_;
};

namespace metric_literals {

// example:
// using namespace metric_literals;
// button->set_pos(200_dp, 200_dp);

inline float operator "" _dp(unsigned long long value)
{
    return value * eps::metrics::instance().density();
}

} /* metric_literals */

} /* eps */


#endif // METRICS_METRICS_H_INCLUDED
