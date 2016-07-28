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

#ifndef UI_CONTROLS_LABEL_H_INCLUDED
#define UI_CONTROLS_LABEL_H_INCLUDED

#include "ui/freetype/forwards.h"
#include "ui/control.h"
#include "rendering/core/program.h"
#include "rendering/core/texture.h"
#include "rendering/primitives/square.h"
#include "math/types.h"

namespace eps {
namespace ui {

class label : public control
{
public:

    explicit label(control * parent = nullptr);

    void draw() override;

    bool set_font(const std::string & ttf_asset, int height);
    bool set_text(const std::string & text);

    void set_color(const math::vec4 & color);
    void set_alignment(halignment haling, valignment valign);

private:

    rendering::program program_face_;
    rendering::texture texture_face_;

    rendering::primitive::square square_;

    freetype::face_handle font_;

    math::vec4 color_;
    int height_;

    valignment valignment_;
    halignment halignment_;
};

} /* ui */
} /* eps */

#endif // UI_CONTROLS_LABEL_H_INCLUDED
