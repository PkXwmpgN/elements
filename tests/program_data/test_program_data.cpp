
#include <assets/assets_storage.h>
#include <rendering/utils/program_data.h>

#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <streambuf>
#include <unordered_map>

/*
<shaders> - ok, use default version - "#version 100"
<shaders version="100_es"> - ok, use 100 es - "#version 100"
<shaders version="300_es"> - ok, use 300 es - "#version 300 es"
<shaders version="310_es"> - ok, use 310 es - "#version 310 es"
<shaders version="asdasda"> - error, can't load a shader
*/

const std::unordered_map<std::string, std::pair<std::string, std::string>> tests_map =
{
    {"default_version.prog", std::make_pair("default_version.vs", "default_version.fs")},
    {"100_es_version.prog", std::make_pair("100_es_version.vs", "100_es_version.fs")},
    {"300_es_version.prog", std::make_pair("300_es_version.vs", "300_es_version.fs")},
    {"310_es_version.prog", std::make_pair("310_es_version.vs", "310_es_version.fs")},
    {"asdasda_version.prog", std::make_pair("asdasda_version.vs", "asdasda_version.fs")}
};

std::string read_from_stream(std::ifstream & stream)
{
    return {std::istreambuf_iterator<char>{stream}, {}};
}

int main()
{
    for (const auto& test : tests_map)
    {
        auto data = eps::assets_storage::instance().read<eps::rendering::program_data>(test.first);

        std::ifstream vs_result(std::get<0>(test.second));

        if (!vs_result.is_open())
        {
            std::cerr << "could not read data from file " << std::get<0>(test.second) << std::endl;
            continue;
        }

        std::string vertex_shader = read_from_stream(vs_result);
        vs_result.close();

        std::ifstream fs_result(std::get<1>(test.second));

        if (!fs_result.is_open())
        {
            std::cerr << "could not read data from file " << std::get<1>(test.second) << std::endl;
            continue;
        }

        std::string fragment_shader = read_from_stream(fs_result);
        fs_result.close();

        if (!data)
        {
            if (vertex_shader.empty() && fragment_shader.empty())
            {
                std::cout << test.first << " with unknown version PASS" << std::endl;
            }
            else
            {
                std::cerr << "could not read data from asset " << test.first << std::endl;
            }

            continue;
        }

        if (vertex_shader.empty() && fragment_shader.empty())
        {
            std::cerr << test.first << " with unknown version FAIL" << std::endl;
        }

        if ((vertex_shader == data.value().v_shader()) && fragment_shader == data.value().f_shader())
        {
            std::cout << test.first << " with known version PASS" << std::endl;
        }
        else
        {
            std::cerr << test.first << " with known version FAIL" << std::endl;
        }
    }
}
