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

#ifndef RENDERING_MODELS_PROCESS_LOAD_MODEL_H_INCLUDED
#define RENDERING_MODELS_PROCESS_LOAD_MODEL_H_INCLUDED

#include "scene/scene.h"
#include "assets/asset_model.h"

namespace eps {
namespace rendering {

struct process_load_model
{
    explicit process_load_model(utils::pointer<scene::scene> scene)
        : scene_(scene)
    {}

    void operator()(scene::node & node, const asset_model & asset);

private:

    utils::pointer<scene::scene> scene_;

};

} /* rendering */
} /* eps */

#endif // RENDERING_MODELS_PROCESS_LOAD_MODEL_H_INCLUDED
