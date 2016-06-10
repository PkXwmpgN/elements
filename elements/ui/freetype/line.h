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

#ifndef UI_FREETYPE_LINE_H_INCLUDED
#define UI_FREETYPE_LINE_H_INCLUDED

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H

#include "forwards.h"
#include "math/types.h"

#include <string>
#include <vector>

namespace eps {
namespace ui {
namespace freetype {

class line
{
public:

    line(FTC_Manager manager,
         FTC_ImageCache image_cache,
         FTC_CMapCache map_cache);

    line(const line &) = delete;
    line & operator=(const line&) = delete;

    line(line&&) = default;
    line & operator=(line&&) = default;

    ~line();

    bool decode(const std::string & text, face_handle handle, int height);

    const math::vec2 & get_size() const { return size_; }
    const void * get_data() const { return data_.data(); }

private:

    FT_UInt  charIndex(face_handle handle, FT_UInt ch);
    FT_Glyph charGlyph(face_handle handle, int height, FT_UInt ch, FT_UInt flags, FTC_Node * node);

    FT_Face sizedFace(face_handle handle, int height);

    struct ft_character
    {
        // UCS2(4) character, suitable for FreeType
        FT_UInt ft_char;

        // Internal character index
        FT_UInt ft_index;

        // Handle for the rendered glyph
        FT_Glyph ft_glyph;

        // Fixed-point character advance and character size
        FT_F26Dot6 ft_advance;
        FT_F26Dot6 ft_width;

        // Cache node for this glyph
        FTC_Node ft_cache_node;

        ft_character(FT_UInt ch = 0)
            : ft_char(ch)
            , ft_index((FT_UInt)(-1))
            , ft_glyph(nullptr)
            , ft_advance(0)
            , ft_width(0)
            , ft_cache_node(nullptr)
        {}

        void advance();
        void kern(const ft_character & right, FT_Face face);
    };

private:

    std::vector<ft_character> chars_;
    std::vector<unsigned char> data_;

    FTC_Manager    ftc_manager_;
    FTC_ImageCache ftc_image_cache_;
    FTC_CMapCache  ftc_map_cache_;

    math::vec2 size_;
};

} /* freetype */
} /* ui */
} /* eps */

#endif // UI_FREETYPE_LINE_H_INCLUDED
