#include <cstdint>
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
    glDeleteVertexArrays(1, &mVertexArrayObject);
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mIndexBuffer);
}

void Mesh::prepareGfxData()
{
    glGenVertexArrays(1, &mVertexArrayObject);
    glGenBuffers(1, &mVertexBuffer);
    glGenBuffers(1, &mIndexBuffer);

    glBindVertexArray(mVertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mData->getVertexBufferSize(), mData->getVertices(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mData->getIndexBufferSize(), mData->getIndices(), GL_STATIC_DRAW);

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_POSITION))
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_POSITION));
    }

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_NORMAL))
    {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_NORMAL));
    }

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_COLOR))
    {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_COLOR));
    }

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_TANGENT))
    {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_TANGENT));
    }

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_JOINTS))
    {
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_INT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_JOINTS));

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_WEIGHTS));
    }

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_TEXCOORD0))
    {
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_TEXCOORD0));
    }

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_TEXCOORD1))
    {
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_TEXCOORD1));
    }

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_TEXCOORD2))
    {
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_TEXCOORD2));
    }

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_TEXCOORD3))
    {
        glEnableVertexAttribArray(9);
        glVertexAttribPointer(9, 2, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_TEXCOORD3));
    }

    glBindVertexArray(0);
}

void Mesh::draw(unsigned int mode)
{
    glBindVertexArray(mVertexArrayObject);
    glDrawElements(mode, this->getIndexCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

VertexLayout* Mesh::getVertexLayout()
{
    return mData->getVertexLayout();
}

uint32_t Mesh::getVertexCount()
{
    return mData->getNumVertices();
}

uint32_t Mesh::getIndexCount()
{
    return mData->getNumIndices();
}

uint32_t Mesh::getLayout()
{
    return mData->getVertexLayout()->getLayout();
}

Mesh* genCubeMesh()
{
    return nullptr;
}

Mesh* genQuadMesh()
{
    uint32_t layout = (uint32_t)SEMANTIC_POSITION;
    layout |= (uint32_t)SEMANTIC_TEXCOORD0;
    layout |= (uint32_t)SEMANTIC_NORMAL;
    VertexLayout* vertexLayout = new VertexLayout(layout);
    MeshData* meshData = new MeshData(4, 0, vertexLayout);
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

    uint32_t indexData[6] = {0, 1, 2, 0, 2, 3};

    meshData->setAttribute(SEMANTIC_POSITION, positionData, 12 * sizeof(float));
    meshData->setAttribute(SEMANTIC_TEXCOORD0, texcoordData, 8 * sizeof(float));
    meshData->setAttribute(SEMANTIC_NORMAL, normalData, 12 * sizeof(float));
    meshData->setIndexes(indexData, 6 * sizeof(uint32_t));
    Mesh* mesh = new Mesh(meshData);
    mesh->prepareGfxData();
    return mesh;
}

EFFECTS_NAMESPACE_END
