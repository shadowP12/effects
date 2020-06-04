#pragma once

#include <vector>
#include "../Base.h"
#include "../Gfx/Gfx.h"
EFFECTS_NAMESPACE_BEGIN

class VertexLayout;
class MeshData;
class GfxBuffer;

class Mesh
{
public:
    Mesh(MeshData* data);
    ~Mesh();
    void prepareGfxData();
    VertexLayout* getVertexLayout();
    uint32_t getVertexCount();
    uint32_t getIndexCount();
    uint32_t getLayout();
    void draw(unsigned int mode);
private:
    MeshData* mData;
    unsigned int mVertexArrayObject;
    unsigned int mVertexBuffer;
    unsigned int mIndexBuffer;
};

Mesh* genCubeMesh();

Mesh* genQuadMesh();

EFFECTS_NAMESPACE_END