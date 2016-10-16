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

#include "program_data.h"

#include <unordered_map>

namespace eps {
namespace rendering {

const std::unordered_map<std::string, std::string> versions_map =
{
    {"100_es", "#version 100\n"},
    {"300_es", "#version 300 es\n"},
    {"310_es", "#version 310 es\n"}
};
const char * default_version_key = "100_es";

bool program_data::read(const pugi::xml_document & doc)
{
    pugi::xml_node root_node = doc.child("program");
    if(root_node.empty())
        return false;

    pugi::xml_node vertex_node = root_node.child("shaders").child("vertex");
    if(vertex_node.text().empty())
        return false;
    pugi::xml_node fragment_node = root_node.child("shaders").child("fragment");
    if(fragment_node.text().empty())
        return false;

    v_shader_.clear();
    f_shader_.clear();

    auto shader_node = root_node.child("shaders");
    const char * key = shader_node.attribute("version").as_string(default_version_key);
    auto it = versions_map.find(key);
    if(it == versions_map.end())
        return false;

    v_shader_ += it->second;
    v_shader_ += vertex_node.text().get();

    f_shader_ += it->second;
    f_shader_ += fragment_node.text().get();

    pugi::xml_node a_locations_node = root_node.child("a_locations");
    for(const auto & value : a_locations_node.children("location"))
    {
        attribute_locations_.emplace_back(value.attribute("name").value(),
                                          value.attribute("index").as_int());
    }

    pugi::xml_node u_locations_node = root_node.child("u_locations");
    for(const auto & value : u_locations_node.children("location"))
    {
        uniform_locations_.emplace_back(value.attribute("name").value(),
                                        value.attribute("index").as_int());
    }

    return true;
}

} /* rendering */
} /* eps */
