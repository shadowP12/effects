#pragma once

#include <vector>
#include "Core/Base.h"
#include "Core/Gfx/Gfx.h"
#include "Resources/Resource.h"
EFFECTS_NAMESPACE_BEGIN

class VertexLayout;
class MeshData;
class GfxBuffer;

class Mesh : public Resource
{
public:
    Mesh(MeshData *data);
    ~Mesh();
    static std::shared_ptr<Mesh> create(const std::string& id, MeshData *data);
    static void remove(std::shared_ptr<Mesh> mesh);
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

std::shared_ptr<Mesh> genCubeMesh();

std::shared_ptr<Mesh> genQuadMesh();

EFFECTS_NAMESPACE_END