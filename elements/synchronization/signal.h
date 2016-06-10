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

#ifndef SYNCHRONIZATION_SIGNAL_H_INCLUDED
#define SYNCHRONIZATION_SIGNAL_H_INCLUDED

#include <mutex>
#include <condition_variable>

namespace eps {
namespace sync {

class signal
{
public:

    signal()
        : recieved_(false)
    {}

    signal(signal &&) = default;
    signal & operator=(signal &&) = default;
    signal(const signal&) = delete;
    signal & operator=(const signal &) = delete;

    template<typename _Perf>
    void send(_Perf perf)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this](){ return !recieved_; });

        perf();

        recieved_ = true;
        condition_.notify_one();
    }

    void send()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this](){ return !recieved_; });
        recieved_ = true;
        condition_.notify_one();
    }

    template<typename _Perf>
    void recv(_Perf perf)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this](){ return recieved_; });

        perf();

        recieved_ = false;
        condition_.notify_one();
    }

    void recv()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this](){ return recieved_; });
        recieved_ = false;
        condition_.notify_one();
    }

private:

    std::mutex mutex_;
    std::condition_variable condition_;
    bool recieved_;
};

} /* sync */
} /* eps */

#endif // SYNCHRONIZATION_SIGNAL_H_INCLUDED
