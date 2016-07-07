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

#include "assets_storage.h"
#include "io/file.h"
#include "io/path.h"
#include <assert.h>
#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>

namespace eps {

struct asset_stream : public io::file
{
    asset_stream(const char * name)
        : stream_(fopen(name, "rb"))
        , size_(0)
    {
        if(stream_)
        {
            struct stat st;
            stat(name, &st);
            size_ = st.st_size;
        }
    }

    ~asset_stream()
    {
        if(stream_)
            fclose(stream_);
    }

    size_t read(void * output, size_t size, size_t count) final
    {
        return stream_ ? fread(output, size, count, stream_) : 0;
    }

    size_t tell() const final
    {
        return stream_ ? ftell(stream_) : 0;
    }

    size_t size() const final
    {
        return size_;
    }

    int seek(size_t offset, int origin) final
    {
        return fseek(stream_, offset, origin);
    }

    void flush() final
    {
        fflush(stream_);
    }

private:

    FILE * stream_;
    size_t size_;
};

struct asset_system : public io::system
{
    io::file * open(const std::string & file) final
    {
        return new asset_stream(file.c_str());
    }
    void close(io::file * file) final
    {
        delete file;
    }
    bool exists(const std::string & file) final
    {
        return access(file.c_str(), R_OK) != -1;
    }
};

utils::unique<assets_storage> assets_storage::instance_ = nullptr;

assets_storage::assets_storage()
    : default_fs_(new asset_system())
{}

utils::optional<std::string> assets_storage::mountpoint(const std::string & resource) const
{
    for(const auto & fs : mount_fs_)
    {
        if(fs.first.size() < resource.size() &&
           resource.find(fs.first) == 0 &&
           resource[fs.first.size()] == io::separator)
        {
            return utils::optional<std::string>(fs.first);
        }

    }
    return utils::optional<std::string>();
}

assets_storage & assets_storage::instance()
{
    if(!instance_)
        instance_.reset(new assets_storage());
    return *instance_;
}

} /* eps */
