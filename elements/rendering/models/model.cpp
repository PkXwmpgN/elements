#include "model.h"
#include "model_warehouse.h"

namespace eps {
namespace rendering {

model::model(const utils::link<scene::node> & parent,
             const std::vector<scene::mesh> & meshes,
             const utils::pointer<model_warehouse> & warehouse)
    : scene::geometry(parent, meshes)
    , warehouse_(warehouse)
{}

utils::link<model_warehouse> model::get_warehouse() const
{
    return warehouse_;
}


} /* rendering */
} /* eps */
