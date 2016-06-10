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

#include "asset_texture.h"
#include "asset_operations.h"
#include <GLES2/gl2.h>
#include <png.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>

namespace eps {

static void read_png_data_callback(png_structp png_ptr,
                                   png_byte * raw_data,
                                   png_size_t read_length)
{
    asset_read_operation * opt = static_cast<asset_read_operation*>(png_get_io_ptr(png_ptr));
    opt->read(raw_data, read_length);
}

static unsigned int png_color_to_gl_color(int color_format)
{
    assert(color_format == PNG_COLOR_TYPE_GRAY ||
           color_format == PNG_COLOR_TYPE_RGB_ALPHA ||
           color_format == PNG_COLOR_TYPE_GRAY_ALPHA);

    switch(color_format)
    {
        case PNG_COLOR_TYPE_GRAY:
            return GL_LUMINANCE;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            return GL_RGBA;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            return GL_LUMINANCE_ALPHA;
    }

    return 0;
}


asset_texture::asset_texture()
    : data_(nullptr, ::free)
    , format_(0)
{}

asset_texture::asset_texture(const std::string & resource)
    : asset(resource)
    , data_(nullptr, ::free)
    , format_(0)
{}

bool asset_texture::load(asset_read_operation * opt)
{
    if(opt == nullptr || opt->size() < 8)
        return false;

    png_byte header[8];
    if(opt->read(header, 8) != 8)
        return false;

    if(png_sig_cmp(header, 0, 8) != 0)
        return false;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if(png_ptr == nullptr)
        return false;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(info_ptr == nullptr)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return false;
    }

    png_set_read_fn(png_ptr, opt, read_png_data_callback);

    if(setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return false;
    }

    png_uint_32 width, height;
    int bit_depth, color_type;

    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 nullptr, nullptr, nullptr);

    // convert transparency to full alpha
    if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    // convert grayscale, if needed.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);

    // convert paletted images, if needed.
    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

    // add alpha channel, if there is none.
    // rationale: GL_RGBA is faster than GL_RGB on many GPUs
    if(color_type == PNG_COLOR_TYPE_PALETTE || color_type == PNG_COLOR_TYPE_RGB)
       png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);

    // ensure 8-bit packing
    if(bit_depth < 8)
       png_set_packing(png_ptr);
    else if (bit_depth == 16)
        png_set_scale_16(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    // read the new color type after updates have been made.
    color_type = png_get_color_type(png_ptr, info_ptr);

    png_size_t row_size = png_get_rowbytes(png_ptr, info_ptr);
    assert(row_size > 0);

    png_byte * raw_image = static_cast<png_byte*>(malloc(row_size * height));
    assert(raw_image != nullptr);

    std::vector<png_byte *> row_ptrs(height);

    for(png_uint_32 i = 0; i < height; ++i)
        row_ptrs[i] = raw_image + i * row_size;

    png_read_image(png_ptr, &row_ptrs[0]);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    data_ = std::unique_ptr<void, void(*)(void*)>(raw_image, ::free);
    format_ = png_color_to_gl_color(color_type);
    size_.x = width;
    size_.y = height;

    return true;
}

} /* eps */
