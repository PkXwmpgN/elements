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

#ifndef PREFERENCES_PREFERENCES_H_INCLUDED
#define PREFERENCES_PREFERENCES_H_INCLUDED

#include <string>
#include <set>
#include "utils/std/pointer.h"

namespace eps {

struct preferences_interface
{
    virtual float get_float(const std::string & key, float default_value) const = 0;
    virtual void  set_float(const std::string & key, float value) = 0;

    virtual ~preferences_interface() {}
};

class preferences
{
public:

    struct on_changed_listener
    {
        virtual void on_changed(const std::string & key, float value) = 0;
        virtual ~on_changed_listener() {}
    };

    template<typename _TPreferences, typename ..._TArgs>
    static void init(_TArgs&& ...args)
    {
        _TPreferences * impl = new _TPreferences(std::forward<_TArgs>(args)...);
        instance_.reset(new preferences(impl));
    }

    static preferences & instance();

    float get_float(const std::string & key, float default_value) const
    {
        return impl_->get_float(key, default_value);
    }
    void set_float(const std::string & key, float value)
    {
        impl_->set_float(key, value);
        on_changed(key, value);
    }

    void register_listener(on_changed_listener * listener);
    void unregister_listener(on_changed_listener * listener);

private:

    explicit preferences(preferences_interface * impl);

    preferences(const preferences&) = delete;
    preferences(preferences&&) = delete;
    preferences & operator=(const preferences&) = delete;
    preferences & operator=(preferences&&) = delete;

    void on_changed(const std::string & key, float value);

private:

    static utils::unique<preferences> instance_;
    utils::unique<preferences_interface>  impl_;

    std::set<on_changed_listener*> listeners_;
};

} /* eps */

#endif // PREFERENCES_PREFERENCES_STORAGE_H_INCLUDED
