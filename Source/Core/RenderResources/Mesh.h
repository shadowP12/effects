#pragma once

#include "../Base.h"

EFFECTS_NAMESPACE_BEGIN

class MeshData;

class Mesh
{
public:
    Mesh(MeshData* data);
    ~Mesh();

private:
    MeshData* mData;
};

EFFECTS_NAMESPACE_END