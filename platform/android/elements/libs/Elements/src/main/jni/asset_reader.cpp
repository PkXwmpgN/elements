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

#include "asset_reader.h"
#include "logging.h"

#include <elements/assets/asset_texture.h>
#include <elements/assets/asset_blob.h>
#include <elements/assets/asset_xml.h>
#include <elements/assets/asset_operations.h>
#include <elements/assets/asset_operations_stream.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

struct asset_read_operation_android : public eps::asset_read_operation
{
public:
    explicit asset_read_operation_android(AAsset * jasset)
        : jasset_(jasset) {}

    ~asset_read_operation_android() final
    {
        AAsset_close(jasset_);
    }

    size_t read(void * output, size_t size) final
    {
        return AAsset_read(jasset_, output, size);
    }

    virtual size_t size() const final
    {
        return AAsset_getLength(jasset_);
    }

private:

    AAsset * jasset_;
};

asset_reader::asset_reader(AAssetManager * mgr)
    : mgr_(mgr)
{}

bool asset_reader::visit(eps::asset_texture & asset)
{
    AAsset * jasset = AAssetManager_open(mgr_, asset.resource().c_str(),
                                         AASSET_MODE_STREAMING);
    if(jasset != nullptr)
    {
        asset_read_operation_android opt(jasset);
        if(asset.load(&opt))
            return true;
        LOGE("[asset_texture] can't load texture asset %s", asset.resource().c_str());
    }
    else
    {
        eps::asset_read_operation_stream opt(asset.resource().c_str());
        if(asset.load(&opt))
            return true;
        LOGE("[asset_texture] can't load texture stream %s", asset.resource().c_str());
    }
    return false;
}

bool asset_reader::visit(eps::asset_blob & asset)
{
    AAsset * jasset = AAssetManager_open(mgr_, asset.resource().c_str(),
                                         AASSET_MODE_STREAMING);
    if(jasset != nullptr)
    {
        asset_read_operation_android opt(jasset);
        if(asset.load(&opt))
            return true;
        LOGE("[asset_blob] can't load blob asset %s", asset.resource().c_str());
    }
    return false;
}

bool asset_reader::visit(eps::asset_xml & asset)
{
    AAsset * jasset = AAssetManager_open(mgr_, asset.resource().c_str(),
                                         AASSET_MODE_STREAMING);
    if(jasset != nullptr)
    {
        asset_read_operation_android opt(jasset);
        if(asset.load(&opt))
            return true;
        LOGE("[asset_xml] can't load xml asset %s", asset.resource().c_str());
    }
    return false;
}
