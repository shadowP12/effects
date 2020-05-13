#include "Mesh.h"
#include "../Datas/MeshData.h"
#include "../Gfx/GfxResources.h"
EFFECTS_NAMESPACE_BEGIN

Mesh::Mesh(MeshData* data)
    :mData(data)
{
}

Mesh::~Mesh()
{
    if(mData)
        delete mData;
    if(mVertexBuffer)
        delete mVertexBuffer;
    if(mIndexBuffer)
        delete mIndexBuffer;
}

void Mesh::prepareGfxData()
{
    GfxBufferDesc vertexBufferDesc;
    vertexBufferDesc.size = mData->getVertexBufferSize();
    vertexBufferDesc.memUsage = GfxMemoryUsageBit::DEVICE;
    vertexBufferDesc.bufferUsage = GfxBufferUsageBit::VERTEX;
    vertexBufferDesc.bufferAccess = GfxBufferAccessBit::WRITE;
    mVertexBuffer = new GfxBuffer(vertexBufferDesc);
    mVertexBuffer->writeData(mData->getVertices(), 0, mData->getVertexBufferSize());

    GfxBufferDesc indexBufferDesc;
    indexBufferDesc.size = mData->getVertexBufferSize();
    indexBufferDesc.memUsage = GfxMemoryUsageBit::DEVICE;
    indexBufferDesc.bufferUsage = GfxBufferUsageBit::INDEX;
    indexBufferDesc.bufferAccess = GfxBufferAccessBit::WRITE;
    mIndexBuffer = new GfxBuffer(indexBufferDesc);
    mIndexBuffer->writeData(mData->getIndices(), 0, mData->getIndexBufferSize());
}

MeshDataDescription* Mesh::getVertexLayout()
{
    return mData->getMeshDesc();
}

GfxBuffer* Mesh::getVertexBuffer()
{
    return mVertexBuffer;
}

GfxBuffer* Mesh::getIndexBuffer()
{
    return mIndexBuffer;
}

uint32_t Mesh::getVertexCount()
{
    return mData->getNumVertices();
}

uint32_t Mesh::getIndexCount()
{
    return mData->getNumIndices();
}

Mesh* genCubeMesh()
{
    return nullptr;
}

Mesh* genQuadMesh()
{
    uint32_t meshAttLayout = (uint32_t)MAL_POSITION;
    meshAttLayout |= (uint32_t)MAL_TEXCOORD0;
    meshAttLayout |= (uint32_t)MAL_NORMAL;
    MeshDataDescription* meshDataDesc = new MeshDataDescription(meshAttLayout);
    MeshData* meshData = new MeshData(4, 0, meshDataDesc);
    float positionData[12] = {
            -1.0f,  1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,
            1.0f, -1.0f, 0.0f};
    float texcoordData[8] = {
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 0.0f};
    float normalData[12] = {
            0.0f,  0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f,  0.0f, 1.0f,
            0.0f, 0.0f, 1.0f};

    meshData->setAttribute(MAS_POSITION, positionData, 12 * sizeof(float));
    meshData->setAttribute(MAS_TEXCOORD, texcoordData, 8 * sizeof(float));
    meshData->setAttribute(MAS_NORMAL, normalData, 12 * sizeof(float));
    Mesh* mesh = new Mesh(meshData);
    mesh->prepareGfxData();
    return mesh;
}

EFFECTS_NAMESPACE_END
