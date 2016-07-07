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

#ifndef IO_UTILS_FILE_GUARD_H_INCLUDED
#define IO_UTILS_FILE_GUARD_H_INCLUDED

#include <string>
#include <cassert>
#include "io/system.h"
#include "io/file.h"
#include "utils/std/pointer.h"

namespace eps {
namespace io {

struct file_guard
{
    file_guard(utils::link<io::system> fs, const std::string & file)
        : fs_(fs)
    {
        if(!fs_.expired())
            stream_ = fs_.lock()->open(file);
    }

    ~file_guard()
    {
        if(!fs_.expired())
            fs_.lock()->close(stream_);
    }

    bool valid() const { return stream_ != nullptr; }

    size_t read(void * output, size_t size, size_t count)
    {
        assert(stream_);
        return stream_->read(output, size, count);
    }

    size_t tell() const
    {
        assert(stream_);
        return stream_->tell();
    }

    size_t size() const
    {
        assert(stream_);
        return stream_->size();
    }

    int seek(size_t offset, int origin)
    {
        assert(stream_);
        return stream_->seek(offset, origin);
    }

    file * get() { return stream_; }

private:

    utils::link<io::system> fs_;
    io::file * stream_ = nullptr;
};

} /* io */
} /* eps */

#endif // IO_UTILS_FILE_GUARD_H_INCLUDED
