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

#include "asset_fs.h"
#include "logging.h"
#include <elements/io/file.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

struct asset_stream : public eps::io::file
{
public:

    explicit asset_stream(AAsset * jasset)
        : jasset_(jasset) {}

    ~asset_stream()
    {
        AAsset_close(jasset_);
    }

    size_t read(void * output, size_t size, size_t count) final
    {
        return AAsset_read(jasset_, output, size * count);
    }

    size_t tell() const final
    {
        return AAsset_getLength(jasset_) - AAsset_getRemainingLength(jasset_);
    }

    size_t size() const final
    {
        return AAsset_getLength(jasset_);
    }

    int seek(size_t offset, int origin) final
    {
        return AAsset_seek(jasset_, offset, origin) == (off_t)-1 ? -1 : 0;
    }

    void flush() final
    {

    }

private:

    AAsset * jasset_;
};

asset_fs::asset_fs(AAssetManager * mgr)
    : mgr_(mgr)
{}

eps::io::file * asset_fs::open(const std::string & file)
{
    if(AAsset * jasset = AAssetManager_open(mgr_, file.c_str(), AASSET_MODE_STREAMING))
        return new asset_stream(jasset);
    return nullptr;
}

bool asset_fs::exists(const std::string & file)
{
    if(AAsset * jasset = AAssetManager_open(mgr_, file.c_str(), AASSET_MODE_STREAMING))
    {
        AAsset_close(jasset);
        return true;
    }
    return false;
}

void asset_fs::close(eps::io::file * file)
{
    delete file;
}
