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

#ifndef SCENE_MODIFIER_POSITIONING_H_INCLUDED
#define SCENE_MODIFIER_POSITIONING_H_INCLUDED

#include "modifier.h"
#include "math/types.h"
#include "math/quaternion.h"

namespace eps {
namespace scene {

class modifier_positioning : public modifier
{
public:

    using modifier::modifier;
    void process(float dt) override;

public:

    void set_position(const math::vec3 & pos) { position_ = pos; }
    void set_rotation(const math::quat & quat) { rotation_ = quat; }
    void set_rotation(const math::vec3 & axis, float degree);

    const math::vec3 & get_position() const { return position_; }
    const math::quat & get_rotation() const { return rotation_; }

    void look_at(const math::vec3 & pos,
                 const math::vec3 & target,
                 const math::vec3 & up);
    void look_at(const math::vec3 & target,
                 const math::vec3 & up);

private:

    math::vec3 position_;
    math::quat rotation_;
};

} /* scene */
} /* eps */

#endif // SCENE_MODIFIER_POSITIONING_H_INCLUDED
