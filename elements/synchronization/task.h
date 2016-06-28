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

#ifndef SYNCHRONIZATION_TASK_H_INCLUDED
#define SYNCHRONIZATION_TASK_H_INCLUDED

#include <thread>
#include <atomic>

#include "future.h"

namespace eps {
namespace sync {

template<typename _ResultType>
class task
{
public:

    using future = sync::future<_ResultType>;

    ~task()
    {
        stop();
    }

    task() = default;
    task(task &&) = default;
    task & operator=(task &&) = default;
    task(const task&) = delete;
    task & operator=(const task &) = delete;

    template<typename _Operation, typename... _Args>
    sync::future<_ResultType> start(size_t size, _Operation && ops, _Args&&... args)
    {
        stop();
        stop_flag_ = false;

        sync::promise<_ResultType> promise(size);
        sync::future<_ResultType> output = promise.get_future();
        controller_ = promise.get_controller();

        std::thread worker([this](auto input, auto && ops, auto&&... args)
        {
            while(!stop_flag_)
            {
                ops(std::begin(input.input()), std::end(input.input()),
                    std::forward<_Args>(args)...);
                input.send();
            }

        }, std::move(promise), std::forward<_Operation>(ops), std::forward<_Args>(args)...);

        worker_ = std::move(worker);
        return output;
    }

    void stop()
    {
        if(controller_.valid())
            controller_.close();

        stop_flag_ = true;
        if(worker_.joinable())
            worker_.join();
    }

private:

    std::atomic<bool> stop_flag_;
    std::thread worker_;

    sync::controller<_ResultType> controller_;
};

} /* sync */
} /* eps */

#endif // SYNCHRONIZATION_TASK_H_INCLUDED
