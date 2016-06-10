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

#ifndef UTILS_STD_PRODUCT_H_INCLUDED
#define UTILS_STD_PRODUCT_H_INCLUDED

namespace eps {
namespace utils {

template<typename _Type, _Type _Default>
class product
{
public:

    using value_type = _Type;
    using value_pointer = _Type*;
    using value_const_pointer = const _Type*;

    static _Type default_value();
    static const product & default_product();

    product();
    explicit product(const value_type & value);
    explicit operator value_const_pointer() const;
    explicit operator value_pointer();

    bool invalid() const;

    friend bool operator==(const product & l, const product & r);
    friend bool operator!=(const product & l, const product & r);

private:

    value_type value_;
};

template<typename _Product>
typename _Product::value_type raw_product(const _Product & object);

template<typename _Product>
typename _Product::value_const_pointer ptr_product(const _Product & object);

template<typename _Product>
typename _Product::value_pointer ptr_product(_Product & object);

template<typename _Type, _Type _Default>
inline _Type product<_Type, _Default>::default_value()
{
    return _Default;
}

template<typename _Type, _Type _Default>
inline const product<_Type, _Default> & product<_Type, _Default>::default_product()
{
    static product<_Type, _Default> def;
    return def;
}

template<typename _Type, _Type _Default>
inline product<_Type, _Default>::product()
    : value_(_Default)
{}

template<typename _Type, _Type _Default>
inline product<_Type, _Default>::product(const value_type & value)
    : value_(value)
{}

template<typename _Type, _Type _Default>
inline product<_Type, _Default>::operator
    product<_Type, _Default>::value_const_pointer() const
{
    return &value_;
}

template<typename _Type, _Type _Default>
inline product<_Type, _Default>::operator
    product<_Type, _Default>::value_pointer()
{
    return &value_;
}

template<typename _Type, _Type _Default>
inline bool product<_Type, _Default>::invalid() const
{
    return value_ == _Default;
}

template<typename _Type, _Type _Default>
inline bool operator==(const product<_Type, _Default> & l,
                       const product<_Type, _Default> & r)
{
    return l.value_ == r.value_;
}

template<typename _Type, _Type _Default>
inline bool operator!=(const product<_Type, _Default> & l,
                       const product<_Type, _Default> & r)
{
    return l.value_ != r.value_;
}

template<typename _Product>
inline typename _Product::value_type raw_product(const _Product & object)
{
    return *static_cast<typename _Product::value_const_pointer>(object);
}

template<typename _Product>
inline typename _Product::value_const_pointer ptr_product(const _Product & object)
{
    return static_cast<typename _Product::value_const_pointer>(object);
}

template<typename _Product>
inline typename _Product::value_pointer ptr_product(_Product & object)
{
    return static_cast<typename _Product::value_pointer>(object);
}

} /* utils */
} /* eps */

#endif // UTILS_STD_PRODUCT_H_INCLUDED
