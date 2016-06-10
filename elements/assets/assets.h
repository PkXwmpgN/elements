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

#ifndef ASSETS_ASSETS_H_INCLUDED
#define ASSETS_ASSETS_H_INCLUDED

#include <string>

namespace eps {

namespace details {

    template<typename... _Types>
    struct asset_visitor;

    template<typename _T>
    struct asset_visitor<_T>
    {
        virtual bool visit(_T & /*asset*/) { return false; }
    };

    template<typename _T, typename... _Types>
    struct asset_visitor<_T, _Types...> : public asset_visitor<_Types...>
    {
        using asset_visitor<_Types...>::visit;
        virtual bool visit(_T & /*asset*/) { return false; }
    };

    template<typename... _Types>
    struct asset_dispatcher
    {
        virtual bool accept(asset_visitor<_Types...> & visitor) = 0;
    };
}

struct asset;
struct asset_texture;
struct asset_blob;
struct asset_xml;

using asset_visitor_base = details::asset_visitor
<
    asset_texture,
    asset_blob,
    asset_xml
>;
using asset_dispatcher_base = details::asset_dispatcher
<
    asset_texture,
    asset_blob,
    asset_xml
>;

using asset_visitor = asset_visitor_base;

template<typename _Asset>
struct asset_dispatcher : public asset_dispatcher_base
{
    bool accept(asset_visitor & visitor) final
    {
        return visitor.visit(*static_cast<_Asset*>(this));
    }
};

struct asset_read_operation;

struct asset
{
    asset()
    {}

    virtual ~asset()
    {}

    explicit asset(const std::string & resource)
        : resource_(resource)
    {}

    asset(const asset &) = delete;
    asset & operator=(const asset &) = delete;
    asset(asset &&) = default;
    asset & operator=(asset&&) = default;

    const std::string & resource() const { return resource_; }

public:

    virtual bool load(asset_read_operation * opt) = 0;

private:

    std::string resource_;
};

} /* eps */

#endif // ASSET_ASSETS_H_INCLUDED
