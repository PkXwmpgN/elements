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

#ifndef ASSETS_ASSET_XML_H_INCLUDED
#define ASSETS_ASSET_XML_H_INCLUDED

#include "assets.h"
#include <memory>
#include <pugixml.hpp>

namespace eps {

struct asset_xml : public asset
{
public:

    using asset::asset;
    bool load(utils::link<io::system> fs, const std::string & resource) final;

private:

    virtual bool read(const pugi::xml_document & doc) = 0;

private:

    std::unique_ptr<pugi::xml_document> doc_;
};

} /* eps */

#endif // ASSETS_ASSET_XML_H_INCLUDED
