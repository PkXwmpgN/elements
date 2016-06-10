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

#ifndef ASSETS_ASSET_TEXTURE_H_INCLUDED
#define ASSETS_ASSET_TEXTURE_H_INCLUDED

#include "assets.h"
#include <memory>
#include "math/types.h"

namespace eps {

struct asset_texture : public asset,
                       public asset_dispatcher<asset_texture>
{
public:

    asset_texture();
    explicit asset_texture(const std::string & resource);

    asset_texture(const asset_texture &) = delete;
    asset_texture & operator=(const asset_texture &) = delete;
    asset_texture(asset_texture &&) = default;
    asset_texture & operator=(asset_texture&&) = default;

    const void * pixels() const { return data_.get(); }
    const math::uvec2 & size() const { return size_; }
    unsigned int format() const { return format_; }

    bool load(asset_read_operation * opt) final;

private:

    std::unique_ptr<void, void(*)(void*)> data_;

    math::uvec2 size_;
    size_t format_;
};


} /* eps */

#endif // ASSETS_ASSET_TEXTURE_H_INCLUDED
