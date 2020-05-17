#include "GfxRender.h"
#include "GfxResources.h"
#include "../Datas/MeshData.h"
EFFECTS_NAMESPACE_BEGIN

static unsigned int gfxPrimitiveModeToGLPrimitiveMode(const GfxPrimitiveMode& gfxPrimitiveMode)
{
    switch (gfxPrimitiveMode)
    {
        case GfxPrimitiveMode::POINT_LIST:
            return GL_POINTS;
        case GfxPrimitiveMode::LINE_LIST:
            return GL_LINES;
        case GfxPrimitiveMode::LINE_STRIP:
            return GL_LINE_STRIP;
        case GfxPrimitiveMode::LINE_LOOP:
            return GL_LINE_LOOP;
        case GfxPrimitiveMode::TRIANGLE_LIST:
            return GL_TRIANGLES;
        case GfxPrimitiveMode::TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
        case GfxPrimitiveMode::TRIANGLE_FAN:
            return GL_TRIANGLE_FAN;
        default:
            return GL_REPEAT;
    }
}

GfxRenderObj::GfxRenderObj()
{
    reset();
}

GfxRenderObj::~GfxRenderObj()
{
}


void GfxRenderObj::setProgram(GfxProgram* program)
{
    mCurProgram = program;
}

void GfxRenderObj::setVertexBuffer(GfxBuffer* buffer)
{
    mCurVertexBuffer = buffer;
}

void GfxRenderObj::setIndexBuffer(GfxBuffer* buffer)
{
    mCurIndexBuffer = buffer;
}

void GfxRenderObj::setVertexLayout(MeshDataDescription* vertexLayout)
{
    mCurVertexLayout = vertexLayout;
}

void GfxRenderObj::draw(const GfxPrimitiveMode& primitiveMode, const int& first, const int& count)
{
    mCurProgram->bind();

    // todo：使用缓存的方式创建vao
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, mCurVertexBuffer->mHandle);
    if (mCurVertexLayout->hasMeshAttribute(MAS_POSITION))
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mCurVertexLayout->getMeshAttributeStride(), (GLvoid*)mCurVertexLayout->getMeshAttributeOffset(MAS_POSITION));
    }

    if (mCurVertexLayout->hasMeshAttribute(MAS_TEXCOORD))
    {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, mCurVertexLayout->getMeshAttributeStride(), (GLvoid*)mCurVertexLayout->getMeshAttributeOffset(MAS_TEXCOORD));
    }

    if (mCurVertexLayout->hasMeshAttribute(MAS_NORMAL))
    {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, mCurVertexLayout->getMeshAttributeStride(), (GLvoid*)mCurVertexLayout->getMeshAttributeOffset(MAS_NORMAL));
    }

    if (mCurVertexLayout->hasMeshAttribute(MAS_TANGENT))
    {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, mCurVertexLayout->getMeshAttributeStride(), (GLvoid*)mCurVertexLayout->getMeshAttributeOffset(MAS_TANGENT));
    }

    if (mCurVertexLayout->hasMeshAttribute(MAS_BLEND_INDICES))
    {
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_INT, GL_FALSE, mCurVertexLayout->getMeshAttributeStride(), (GLvoid*)mCurVertexLayout->getMeshAttributeOffset(MAS_BLEND_INDICES));

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mCurVertexLayout->getMeshAttributeStride(), (GLvoid*)mCurVertexLayout->getMeshAttributeOffset(MAS_BLEND_WEIGHTS));
    }

    glDrawArrays(gfxPrimitiveModeToGLPrimitiveMode(primitiveMode), first, count);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
}

void GfxRenderObj::drawIndexed(const GfxPrimitiveMode& primitiveMode, const int& indexCount)
{
    mCurProgram->bind();
    // todo：使用缓存的方式创建vao
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, mCurVertexBuffer->mHandle);

    if (mCurVertexLayout->hasMeshAttribute(MAS_POSITION))
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mCurVertexLayout->getMeshAttributeStride(), (GLvoid*)mCurVertexLayout->getMeshAttributeOffset(MAS_POSITION));
    }

    if (mCurVertexLayout->hasMeshAttribute(MAS_TEXCOORD))
    {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, mCurVertexLayout->getMeshAttributeStride(), (GLvoid*)mCurVertexLayout->getMeshAttributeOffset(MAS_TEXCOORD));
    }

    if (mCurVertexLayout->hasMeshAttribute(MAS_NORMAL))
    {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, mCurVertexLayout->getMeshAttributeStride(), (GLvoid*)mCurVertexLayout->getMeshAttributeOffset(MAS_NORMAL));
    }

    if (mCurVertexLayout->hasMeshAttribute(MAS_TANGENT))
    {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, mCurVertexLayout->getMeshAttributeStride(), (GLvoid*)mCurVertexLayout->getMeshAttributeOffset(MAS_TANGENT));
    }

    if (mCurVertexLayout->hasMeshAttribute(MAS_BLEND_INDICES))
    {
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_INT, GL_FALSE, mCurVertexLayout->getMeshAttributeStride(), (GLvoid*)mCurVertexLayout->getMeshAttributeOffset(MAS_BLEND_INDICES));

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mCurVertexLayout->getMeshAttributeStride(), (GLvoid*)mCurVertexLayout->getMeshAttributeOffset(MAS_BLEND_WEIGHTS));
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCurIndexBuffer->mHandle);

    glDrawElements(gfxPrimitiveModeToGLPrimitiveMode(primitiveMode), indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
}

void GfxRenderObj::reset()
{
    mCurProgram = nullptr;
    mCurVertexBuffer = nullptr;
    mCurIndexBuffer = nullptr;
    mCurVertexLayout = nullptr;
}

EFFECTS_NAMESPACE_END