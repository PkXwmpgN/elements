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

#ifndef UTILS_STD_POINTER_H_INCLUDED
#define UTILS_STD_POINTER_H_INCLUDED

#include <memory>

namespace eps {
namespace utils {

template<typename _Type>
using pointer = std::shared_ptr<_Type>;

template<typename _Type>
using link = std::weak_ptr<_Type>;

template<typename _Type, typename _Deleter = std::default_delete<_Type>>
using unique = std::unique_ptr<_Type, _Deleter>;

template<typename _Derived>
using enable_shared_from_this = std::enable_shared_from_this<_Derived>;

template<typename _Type, typename... _Args>
inline pointer<_Type> make_shared(_Args&&... args)
{
    return std::make_shared<_Type>(std::forward<_Args>(args)...);
}

template<typename _Type, typename... _Args>
inline unique<_Type> make_unique(_Args&&... args)
{
    return std::make_unique<_Type>(std::forward<_Args>(args)...);
}

template<typename _To, typename _From>
inline pointer<_To> dynamic_pointer_cast(const pointer<_From> & from)
{
    return std::dynamic_pointer_cast<_To>(from);
}

} /* utils */
} /* eps */

#endif // UTILS_STD_POINTER_H_INCLUDED
