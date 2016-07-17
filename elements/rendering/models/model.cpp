#include "model.h"
#include "model_warehouse.h"

namespace eps {
namespace rendering {

model::model(utils::link<scene::node> parent,
             utils::pointer<model_warehouse> warehouse)
    : scene::object(parent)
    , warehouse_(warehouse)
{}

utils::link<model_warehouse> model::get_warehouse() const
{
    return warehouse_;
}

} /* rendering */
} /* eps */
