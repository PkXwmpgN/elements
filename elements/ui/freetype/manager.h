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

#ifndef UI_TEXT_manager_H_INCLUDED
#define UI_TEXT_manager_H_INCLUDED

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H

#include "forwards.h"
#include "assets/asset_blob.h"

#include <string>
#include <vector>
#include <map>

namespace eps {
namespace ui {
namespace freetype {

class manager
{
public:

    static manager & instance();

    face_handle get_face(const std::string & ttf);
    line get_line(const std::string & text, face_handle handle, int height) const;

private:

    manager();
    ~manager();

    manager(const manager&) = delete;
    manager(manager&&) = delete;

    manager & operator=(const manager&) = delete;
    manager & operator=(manager&&) = delete;

    bool startup();
    void cleanup();

    face_handle load_face(const std::string & ttf);

    static FT_Error FreeType_Face_Requester(FTC_FaceID faceID,
                                            FT_Library library,
                                            FT_Pointer data,
                                            FT_Face* face);
private:

    std::vector<asset_blob> face_assets_;
    std::map<std::string, FT_Face> face_handles_;

    FT_Library ft_library_;

    FTC_Manager    ftc_manager_;
    FTC_ImageCache ftc_image_cache_;
    FTC_CMapCache  ftc_map_cache_;
};

} /* freetyep */
} /* ui */
} /* eps */

#endif // UI_TEXT_manager_H_INCLUDED
