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

#include "line.h"
#include FT_SIZES_H

#include <algorithm>

namespace eps {
namespace ui {
namespace freetype {

void line::ft_character::advance()
{
   ft_advance = 0;
   ft_width = 0;

   if(ft_glyph)
   {
       ft_advance = ft_glyph->advance.x >> 10;
       FT_BBox bbox;
       FT_Glyph_Get_CBox(ft_glyph, FT_GLYPH_BBOX_GRIDFIT, &bbox);
       ft_width = bbox.xMax - bbox.xMin;
       if(ft_width == 0 && ft_advance != 0)
           ft_width = ft_advance;
   }
}

void line::ft_character::kern(const ft_character & right, FT_Face face)
{
    if(ft_index && right.ft_index)
    {
        FT_Vector delta;
        FT_Get_Kerning(face, ft_index, right.ft_index, FT_KERNING_DEFAULT, &delta);

        ft_advance += delta.x;
    }
}

line::line(FTC_Manager manager, FTC_ImageCache image_cache, FTC_CMapCache map_cache)
    : ftc_manager_(manager)
    , ftc_image_cache_(image_cache)
    , ftc_map_cache_(map_cache)
{}

line::~line()
{
    if(ftc_manager_)
    {
        for(ft_character & c : chars_)
        {
            if(c.ft_cache_node)
            {
                FTC_Node_Unref(c.ft_cache_node, ftc_manager_);
                c.ft_cache_node = nullptr;
            }
        }
    }
}

bool line::line::decode(const std::string & text, face_handle handle, int height)
{
    FT_Face face = sizedFace(handle, height);
    if(face == nullptr)
        return false;

    bool use_kerning = FT_HAS_KERNING(face);

    // todo : decode from utf8 to ucs2(4)
    chars_.assign(text.begin(), text.end());

    for(auto it = chars_.begin(); it != chars_.end(); ++it)
    {
        FT_Int ch = it->ft_char;
        it->ft_index = (ch != '\r' && ch != '\n') ? charIndex(handle, ch) : 0;
        it->ft_glyph = charGlyph(handle, height, ch, FT_LOAD_RENDER, &it->ft_cache_node);
        if(it->ft_glyph)
        {
            it->advance();
            if(it != chars_.begin() && use_kerning)
                std::prev(it)->kern(*it, face);
        }
    }

    int result_min_y = -1000;
    int result_max_y = -1000;

    int cx = 0;
    for(const auto & ch : chars_)
    {
        if(ch.ft_glyph)
        {
            FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)ch.ft_glyph;

            cx += ch.ft_advance;

            int y = bitmap_glyph->top;
            int h = bitmap_glyph->bitmap.rows;
            if(result_min_y < y)
                result_min_y = y;
            if(result_max_y < h - y)
                result_max_y = h - y;
        }
    }

    size_.x = cx >> 6;
    size_.y = result_min_y + result_max_y;

    if(!size_.x || !size_.y)
        return false;

    int x = 0;
    int y = result_min_y;

    data_.resize(size_.x * size_.y);
    std::fill(data_.begin(), data_.end(), 0);

    for(auto it = chars_.begin(); it != chars_.end(); ++it)
    {
        if(it->ft_glyph)
        {
            FT_BitmapGlyph bitmap_gpyph = (FT_BitmapGlyph)it->ft_glyph;
            int in_x = (x >> 6) + bitmap_gpyph->left;
            int in_y = y - bitmap_gpyph->top;

            FT_Bitmap * glyph = &bitmap_gpyph->bitmap;

            for(size_t y0 = 0; y0 < glyph->rows; ++y0)
            {
                unsigned char * src = std::next(glyph->buffer, y0 * glyph->pitch);
                unsigned char * dest = std::next(data_.data(), (y0 + in_y) * size_.x + in_x);
                std::copy(src, std::next(src, glyph->pitch), dest);
            }

        }
        x += it->ft_advance;
    }

    return true;
}

FT_Face line::sizedFace(face_handle handle, int height)
{
    FTC_ScalerRec scaler;

    scaler.face_id = handle;
    scaler.height = height;
    scaler.width = 0;
    scaler.pixel = 1;

    FT_Size sized_face;

    if(FTC_Manager_LookupSize(ftc_manager_, &scaler, &sized_face) != 0)
        return nullptr;

    if(FT_Activate_Size(sized_face) != 0)
        return nullptr;

    return sized_face->face;
}

FT_UInt line::charIndex(face_handle handle, FT_UInt ch)
{
    return FTC_CMapCache_Lookup(ftc_map_cache_, handle, -1, ch);
}

FT_Glyph line::charGlyph(face_handle handle, int height, FT_UInt ch,
                            FT_UInt flags, FTC_Node * node)
{
    FT_UInt index = charIndex(handle, ch);

    FTC_ImageTypeRec type;

    type.face_id = handle;
    type.height = height;
    type.width = 0;
    type.flags = flags;

    FT_Glyph glyph;

    if(FTC_ImageCache_Lookup(ftc_image_cache_, &type, index, &glyph, node) != 0)
        return nullptr;

    return glyph;
}

} /* freetype */
} /* ui */
} /* eps */
