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

#include "asset_xml.h"
#include "io/utils/file_guard.h"

namespace eps {

bool asset_xml::load(utils::link<io::system> fs, const std::string & resource)
{
    io::file_guard stream(fs, resource);
    if(!stream.valid())
        return false;

    char * data = static_cast<char*>(pugi::get_memory_allocation_function()(stream.size()));
    if(stream.read(data, 1, stream.size()) != stream.size())
        return false;

    doc_.reset(new pugi::xml_document());
    if(!doc_->load_buffer_inplace_own(data, stream.size()))
        return false;

    return read(*doc_);
}

} /* eps */
