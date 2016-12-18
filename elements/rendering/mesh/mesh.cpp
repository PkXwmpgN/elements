#include "mesh.h"
#include "mesh_storage.h"

namespace eps {
namespace rendering {

mesh::mesh(const utils::link<scene::node> & parent,
           const utils::pointer<mesh_storage> & storage)
    : scene::entity(parent)
    , storage_(storage)
{}

utils::link<mesh_storage> mesh::get_storage() const
{
    return storage_;
}

} /* rendering */
} /* eps */
