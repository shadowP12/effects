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
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mIndexBuffer);
    glDeleteVertexArrays(1, &mVertexBufferArray);
}

void Mesh::initialize()
{
    glGenVertexArrays(1, &mVertexBufferArray);
    glGenBuffers(1, &mVertexBuffer);
    glGenBuffers(1, &mIndexBuffer);

    glBindVertexArray(mVertexBufferArray);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mData->getIndexBufferSize(), mData->getIndices(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mData->getVertexBufferSize(), mData->getVertices(), GL_STATIC_DRAW);

    if (mData->getMeshDesc()->hasMeshAttribute(MAS_POSITION))
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (GLvoid*)0);
    }

    if (mData->getMeshDesc()->hasMeshAttribute(MAS_TEXCOORD))
    {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getMeshDesc()->getMeshAttributeOffset(MAS_TEXCOORD));
    }

    if (mData->getMeshDesc()->hasMeshAttribute(MAS_NORMAL))
    {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getMeshDesc()->getMeshAttributeOffset(MAS_NORMAL));
    }

    if (mData->getMeshDesc()->hasMeshAttribute(MAS_TANGENT))
    {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getMeshDesc()->getMeshAttributeOffset(MAS_TANGENT));
    }

    if (mData->getMeshDesc()->hasMeshAttribute(MAS_BLEND_INDICES))
    {
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_INT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getMeshDesc()->getMeshAttributeOffset(MAS_BLEND_INDICES));

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getMeshDesc()->getMeshAttributeOffset(MAS_BLEND_WEIGHTS));
    }

    glBindVertexArray(0);
}

GLuint Mesh::getVertexBufferArray()
{
    return mVertexBufferArray;
}

GLuint Mesh::getIndexBuffer()
{
    return mIndexBuffer;
}

uint32_t Mesh::getIndexCount()
{
    return mData->getNumIndices();
}

void Mesh::draw()
{
    glBindVertexArray(mVertexBufferArray);
    glDrawElements(GL_TRIANGLES, mData->getNumIndices(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

EFFECTS_NAMESPACE_END
