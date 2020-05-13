#pragma once

#include <vector>
#include "../Base.h"
#include "../Gfx/Gfx.h"
EFFECTS_NAMESPACE_BEGIN

class MeshDataDescription;
class MeshData;
class GfxBuffer;

class Mesh
{
public:
    Mesh(MeshData* data);
    ~Mesh();
    void prepareGfxData();
    MeshDataDescription* getVertexLayout();
    GfxBuffer* getVertexBuffer();
    GfxBuffer* getIndexBuffer();
    uint32_t getVertexCount();
    uint32_t getIndexCount();
private:
    MeshData* mData;
    GfxBuffer* mVertexBuffer;
    GfxBuffer* mIndexBuffer;
};

Mesh* genCubeMesh();

Mesh* genQuadMesh();

EFFECTS_NAMESPACE_END