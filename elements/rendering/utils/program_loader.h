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

#ifndef RENDERING_UTILS_SHADER_PROGRAM_LOADER_H_INCLUDED
#define RENDERING_UTILS_SHADER_PROGRAM_LOADER_H_INCLUDED

#include <string>
#include <vector>
#include "utils/std/iterator_range.h"
#include "assets/asset_xml.h"

namespace eps {
namespace rendering {

class program;

class program_data : public asset_xml
{
public:

    struct location
    {
        location(const char * n, short ind)
            : name(n)
            , index(ind)
        {}

        const char * name;
        short index;
    };

    using locations = std::vector<location>;
    using locations_range = utils::iterator_range<typename locations::const_iterator>;

public:

    using asset_xml::asset_xml;

    const char * v_shader() const { return v_shader_; }
    const char * f_shader() const { return f_shader_; }

    locations_range a_locations() const
    {
        return utils::make_range(attribute_locations_.begin(),
                                 attribute_locations_.end());
    }

    locations_range u_locations() const
    {
        return utils::make_range(uniform_locations_.begin(),
                                 uniform_locations_.end());
    }

private:

    bool read(const pugi::xml_document & doc) final;

private:

    locations attribute_locations_;
    locations uniform_locations_;

    const char * v_shader_ = nullptr;
    const char * f_shader_ = nullptr;
};

bool load_program(const char * program_asset, program & result);

} /* rendering */
} /* eps*/

#endif // RENDERING_UTILS_SHADER_PROGRAM_LOADER_H_INCLUDED
