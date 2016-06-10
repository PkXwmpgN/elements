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

#ifndef ASSETS_ASSET_OPERATIONS_STREAM_H_INCLUDED
#define ASSETS_ASSET_OPERATIONS_STREAM_H_INCLUDED

#include "asset_operations.h"
#include <stdio.h>

namespace eps {

struct asset_read_operation_stream : public asset_read_operation
{
    explicit asset_read_operation_stream(const char * name);
    ~asset_read_operation_stream();

    size_t read(void * output, size_t size) final;
    size_t size() const final;

private:

    FILE * stream_;
    size_t size_;
};

} /* eps */

#endif // ASSETS_ASSET_HANDLE_STREAM_H_INCLUDED
