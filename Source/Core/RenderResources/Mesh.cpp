#include "Mesh.h"
#include "../Datas/MeshData.h"
EFFECTS_NAMESPACE_BEGIN

Mesh::Mesh(MeshData* data)
    :mData(data)
{
}

Mesh::~Mesh()
{
    if(mData)
        delete mData;
}

EFFECTS_NAMESPACE_END
