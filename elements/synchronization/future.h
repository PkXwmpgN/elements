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

#ifndef SYNCHRONIZATION_FUTURE_H_INCLUDED
#define SYNCHRONIZATION_FUTURE_H_INCLUDED

#include <vector>
#include <memory>
#include <assert.h>

#include "signal.h"

namespace eps {
namespace sync {

template<typename _Type> class transport;
template<typename _Type> class future;
template<typename _Type> class promice;

template<typename _Type>
struct traits
{
    using transport_type  = transport<_Type>;
    using ptransport_type = std::shared_ptr<transport_type>;

    using container_type  = std::vector<_Type>;
    using pcontainer_type = std::shared_ptr<container_type>;
};

template<typename _Type>
class promice
{
public:

    using container_type = typename traits<_Type>::container_type;

    explicit promice(size_t size)
        : transport_(new typename traits<_Type>::transport_type(size))
    {}

    promice(promice &&) = default;
    promice & operator=(promice &&) = default;
    promice(const promice &) = delete;
    promice & operator=(const promice &) = delete;

    void send()
    {
        assert(transport_ != nullptr);
        transport_->send();
    }

    bool valid() const
    {
        return transport_ && transport_.use_count() > 1;
    }

    container_type & input()
    {
        assert(transport_ != nullptr);
        return transport_->input();
    }

    future<_Type> get_future() const
    {
        return future<_Type>(transport_);
    }

private:

    typename traits<_Type>::ptransport_type transport_;
};

template<typename _Type>
class future
{
public:

    using container_type = typename traits<_Type>::container_type;

    future() = default;
    future(future &&) = default;
    future & operator=(future &&) = default;
    future(const future &) = delete;
    future & operator=(const future &) = delete;

    explicit future(typename traits<_Type>::ptransport_type transport)
        : transport_(transport)
    {}

    void wait()
    {
        assert(transport_ != nullptr);
        transport_->wait();
    }

    template<typename _Perf>
    void wait(_Perf perf)
    {
        assert(transport_ != nullptr);
        transport_->wait(perf);
    }

    bool valid() const
    {
        return transport_ != nullptr;
    }

    const container_type & output() const
    {
        assert(transport_ != nullptr);
        return transport_->output();
    }

private:

    typename traits<_Type>::ptransport_type transport_;
};

template<typename _Type>
class transport
{
public:

    using container_type = typename traits<_Type>::container_type;

    explicit transport(size_t size)
        : input_(size)
        , output_(size)
    {}

    transport(transport &&) = default;
    transport & operator=(transport &&) = default;
    transport(const transport &) = delete;
    transport & operator=(const transport &) = delete;

    void send()
    {
        customer_.send([this]()
        {
            std::swap(input_, output_);
        });
    }

    template<typename _Perf>
    void wait(_Perf perf)
    {
        customer_.recv(perf);
    }

    void wait()
    {
        customer_.recv();
    }

    container_type & input() { return input_; }
    const container_type & output() const { return output_; }


private:

    signal customer_;

    typename traits<_Type>::container_type input_;
    typename traits<_Type>::container_type output_;
};

} /* sync */
} /* eps */

#endif // SYNCHRONIZATION_FUTURE_H_INCLUDED
