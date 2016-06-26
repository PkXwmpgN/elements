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

#ifndef ASSETS_STORAGE_H_INCLUDED
#define ASSETS_STORAGE_H_INCLUDED

#include <string>
#include <memory>

#include "assets.h"

namespace eps {

struct asset_reader : public asset_visitor {};

class assets_storage
{
public:

    template<typename _TReader, typename ..._TArgs>
    static void init(_TArgs&& ...args)
    {
        _TReader * reader = new _TReader(std::forward<_TArgs>(args)...);
        instance_.reset(new assets_storage(reader));
    }

    template<typename _TAsset>
    _TAsset read(const std::string & resource)
    {
        _TAsset asset(resource);
        asset.accept(*reader_);
        return std::move(asset);
    }

    static assets_storage & instance();

private:

    explicit assets_storage(asset_reader * reader);

    assets_storage(const assets_storage&) = delete;
    assets_storage(assets_storage&&) = delete;
    assets_storage & operator=(const assets_storage&) = delete;
    assets_storage & operator=(assets_storage&&) = delete;

private:

    static std::unique_ptr<assets_storage> instance_;
    std::unique_ptr<asset_reader> reader_;
};

} /* eps */

#endif // ASSETS_STORAGE_H_INCLUDED
