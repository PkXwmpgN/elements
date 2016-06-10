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

#ifndef EXPERIMETS_STRANGE_ATTRACTORS_H_INCLUDED
#define EXPERIMETS_STRANGE_ATTRACTORS_H_INCLUDED

#include <random>

namespace eps {
namespace experiment {
namespace strange {

class attractors
{
public:

    attractors();

    void process(float dt);

    float a() const { return attractors_[0].value; }
    float b() const { return attractors_[1].value; }
    float c() const { return attractors_[2].value; }
    float d() const { return attractors_[3].value; }

private:

    struct data
    {
        data()
            : value(0.0f), target(0.0), inc(0.0f)
        {}

        void update(float next);

        float value;
        float target;
        float inc;
    };

    data attractors_[4];

    std::uniform_real_distribution<float> distribution_;
    std::default_random_engine generator_;
};

} /* strange */
} /* experiment */
} /* eps */

#endif // EXPERIMETS_STRANGE_ATTRACTORS_H_INCLUDED
