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

#ifndef RENDERING_TECHNIQUES_PROJECTOR_PROCESS_H_INCLUDED
#define RENDERING_TECHNIQUES_PROJECTOR_PROCESS_H_INCLUDED

#include "rendering/core/program.h"
#include "rendering/core/texture.h"
#include "rendering/models/model.h"
#include "scene/scene.h"

namespace eps {
namespace rendering {
namespace techniques {

class projector_process : public scene::visitor<projector_process>
{
public:

    SNAPE_VISIT(model);

public:

    void process();
    bool initialize();
    void visit(const model & m);

    void set_scene(const utils::pointer<scene::scene> & scene);

    bool set_projective_map(const std::string & asset);
    void set_projective_camera(const std::string & camera);

private:

    program program_;
    texture texture_projective_;
    utils::pointer<scene::scene> scene_;

    std::string camera_projective_;
};

} /* techniques */
} /* rendering */
} /* eps */

#endif // RENDERING_TECHNIQUES_PROJECTOR_PROCESS_H_INCLUDED
