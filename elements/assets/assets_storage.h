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
#include <unordered_map>

#include "assets.h"

#include "utils/std/pointer.h"
#include "utils/std/optional.h"

#include "io/system.h"

namespace eps {

class assets_storage
{
    using system_pointer = utils::pointer<io::system>;
    using system_link = utils::link<io::system>;
    using system_map = std::unordered_map<std::string, system_pointer>;

public:

    static assets_storage & instance();

    template<typename _TFS, typename ..._TArgs>
    void mount(const std::string & mountpoint, _TArgs&& ...args);

    template<typename _TAsset>
    utils::optional<_TAsset> read(const std::string & resource);

    utils::optional<std::string> mountpoint(const std::string & resource) const;

private:

    assets_storage();
    assets_storage(const assets_storage&) = delete;
    assets_storage(assets_storage&&) = delete;
    assets_storage & operator=(const assets_storage&) = delete;
    assets_storage & operator=(assets_storage&&) = delete;


private:

    static utils::unique<assets_storage> instance_;
    system_map mount_fs_;
    system_pointer default_fs_;
};

template<typename _TFS, typename ..._TArgs>
void assets_storage::mount(const std::string & mountpoint, _TArgs&& ...args)
{
    mount_fs_[mountpoint].reset(new _TFS(std::forward<_TArgs>(args)...));
}

template<typename _TAsset>
utils::optional<_TAsset> assets_storage::read(const std::string & resource)
{
    auto fs_link = default_fs_;
    std::string relative = resource;
    if(auto point = mountpoint(resource))
    {
        fs_link = mount_fs_[point.value()];
        relative.assign(std::next(resource.begin(), point.value().size() + 1),
                        resource.end());
    }

    _TAsset asset(relative);
    if(asset.load(fs_link, relative))
        return utils::optional<_TAsset>(std::move(asset));

    return utils::optional<_TAsset>();
}

} /* eps */

#endif // ASSETS_STORAGE_H_INCLUDED
