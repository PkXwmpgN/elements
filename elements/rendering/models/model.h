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

#ifndef RENDERING_MODELS_MODEL_H_INCLUDED
#define RENDERING_MODELS_MODEL_H_INCLUDED

#include "scene/entity/geometry.h"

namespace eps {
namespace rendering {

class model_warehouse;

class model : public scene::geometry
{
public:

    EPS_DESIGN_VISITABLE();

public:

    model(const utils::link<scene::node> & parent,
          const std::vector<scene::mesh> & meshes,
          const utils::pointer<model_warehouse> & warehouse);

    utils::link<model_warehouse> get_warehouse() const;

private:

    utils::pointer<model_warehouse> warehouse_;
};

} /* rendering */
} /* eps */

#endif // RENDERING_MODELS_MODEL_H_INCLUDED
