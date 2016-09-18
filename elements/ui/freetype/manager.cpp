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

#include "manager.h"
#include "line.h"

#include "assets/assets_storage.h"

namespace eps {
namespace ui {
namespace freetype {

manager::manager()
    : ft_library_(nullptr)
    , ftc_manager_(nullptr)
    , ftc_image_cache_(nullptr)
    , ftc_map_cache_(nullptr)
{
    startup();
}

manager::~manager()
{
    cleanup();
}

manager & manager::instance()
{
    static manager manager;
    return manager;
}

face_handle manager::get_face(const std::string & ttf)
{
    if(face_handles_.count(ttf))
        return face_handles_[ttf];

    return load_face(ttf);
}

line manager::get_line(const std::string & text, face_handle handle, int height) const
{
    line result(ftc_manager_, ftc_image_cache_, ftc_map_cache_);
    result.decode(text, handle, height);
    return result;
}

face_handle manager::load_face(const std::string & ttf)
{
    auto ttf_blob = assets_storage::instance().read<asset_blob>(ttf);
    if(!ttf_blob)
        return INVALID_FACE_HANDLE;

    if(ttf_blob.value().size() == 0)
        return INVALID_FACE_HANDLE;

    FT_Face face;
    if(FT_New_Memory_Face(ft_library_, (const FT_Byte* )ttf_blob.value().data(),
                          (FT_Long)ttf_blob.value().size(), 0, &face) != 0)
        return INVALID_FACE_HANDLE;

    face_handles_[ttf] = face;
    face_assets_.push_back(std::move(ttf_blob.value()));

    return face;
}

FT_Error manager::FreeType_Face_Requester(FTC_FaceID id,
                                          FT_Library /*library*/,
                                          FT_Pointer /*data*/,
                                          FT_Face* face)
{
    *face = reinterpret_cast<FT_Face>(id);
    return 0;
}

bool manager::startup()
{
    if(FT_Init_FreeType(&ft_library_) != 0 )
        return false;

    if(FTC_Manager_New(ft_library_, 0, 0, 0, FreeType_Face_Requester, this, &ftc_manager_) != 0)
        return false;

    if(FTC_ImageCache_New(ftc_manager_, &ftc_image_cache_) != 0)
        return false;

    if(FTC_CMapCache_New(ftc_manager_, &ftc_map_cache_) != 0)
        return false;

    return true;
}

void manager::cleanup()
{
    if(ftc_manager_)
        FTC_Manager_Done(ftc_manager_);

    if(ft_library_)
        FT_Done_FreeType(ft_library_);
}

} /* freetype */
} /* ui */
} /* eps */
