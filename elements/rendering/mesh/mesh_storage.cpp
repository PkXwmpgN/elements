#include "mesh_storage.h"

#include "assets/assets_storage.h"
#include "assets/asset_texture.h"
#include "rendering/core/texture_maker.h"
#include "rendering/core/texture_policy.h"

namespace eps {
namespace rendering {

mesh_storage::geometry_product::geometry_product(const std::vector<scene::vertex> & vertices,
                                                 const std::vector<scene::face> & faces)
{
    vertices_.allocate(vertices.data(), vertices.size(), sizeof(scene::vertex));
    indices_.allocate(faces.data(), faces.size() * scene::face::FACE_INDICES_COUNT,
                      sizeof(unsigned short));
}

void mesh_storage::maps_product::set_diffuse(const product_type & map)
{
    map_diffuse_ = map;
}

void mesh_storage::maps_product::set_specular(const product_type & map)
{
    map_specular_ = map;
}

void mesh_storage::maps_product::set_normal(const product_type & map)
{
    map_normal_ = map;
}

const utils::optional<product_type> & mesh_storage::maps_product::get_diffuse() const
{
    return map_diffuse_;
}

const utils::optional<product_type> & mesh_storage::maps_product::get_specular() const
{
    return map_specular_;
}

const utils::optional<product_type> & mesh_storage::maps_product::get_normal() const
{
    return map_normal_;
}

void mesh_storage::colors_product::set_diffuse(const math::vec3 & color)
{
    color_diffuse_ = color;
}

void mesh_storage::colors_product::set_specular(const math::vec3 & color)
{
    color_specular_ = color;
}

void mesh_storage::colors_product::set_ambient(const math::vec3 & color)
{
    color_ambient_ = color;
}

const utils::optional<math::vec3> & mesh_storage::colors_product::get_diffuse() const
{
    return color_diffuse_;
}

const utils::optional<math::vec3> & mesh_storage::colors_product::get_specular() const
{
    return color_specular_;
}

const utils::optional<math::vec3> & mesh_storage::colors_product::get_ambient() const
{
    return color_ambient_;
}

size_t mesh_storage::add_geometry(geometry_product && geometry)
{
    size_t index = geometry_.size();
    geometry_.push_back(std::move(geometry));
    return index;
}

size_t mesh_storage::add_maps(maps_product && map)
{
    size_t index = maps_.size();
    maps_.push_back(std::move(map));
    return index;
}

size_t mesh_storage::add_colors(colors_product && colors)
{
    size_t index = colors_.size();
    colors_.push_back(std::move(colors));
    return index;
}

const product_type & mesh_storage::register_map(const std::string & name)
{
    auto & texture = maps_cache_[name];
    if(!texture.valid())
    {
        if(auto asset = assets_storage::instance().read<asset_texture>(name))
        {
            auto maker = get_texture_maker<repeat_texture_policy>();
            texture = maker.construct(asset->pixels(), asset->size());
        }
    }
    return texture.get_product();
}

} /* rendering */
} /* eps */
