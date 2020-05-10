#include "GfxCommandBuffer.h"
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

GfxCommandBuffer::GfxCommandBuffer()
{
    reset();
}

GfxCommandBuffer::~GfxCommandBuffer()
{
}

void GfxCommandBuffer::setViewport(const int& x, const int& y, const int& w, const int& h)
{
    glViewport(x, y, w, h);
}

void GfxCommandBuffer::setClearValue(const float& r, const float& g, const float& b, const float& a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GfxCommandBuffer::setProgram(GfxProgram* program)
{
    mProgram = program;
}

void GfxCommandBuffer::setVertexBuffer(GfxBuffer* buffer)
{
    mVertexBuffer = buffer;
}

void GfxCommandBuffer::setIndexBuffer(GfxBuffer* buffer)
{
    mIndexBuffer = buffer;
}

void GfxCommandBuffer::setVertexLayout(MeshDataDescription* vertexLayout)
{
    mVertexLayout = vertexLayout;
}

void GfxCommandBuffer::bindFramebuffer(GfxFramebuffer* fb)
{
    if(!fb)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }
    fb->bind();
}

void GfxCommandBuffer::draw(const GfxPrimitiveMode& primitiveMode, const int& first, const int& count)
{
    mProgram->bind();

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer->mHandle);

    if (mVertexLayout->hasMeshAttribute(MAS_POSITION))
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (GLvoid*)0);
    }

    if (mVertexLayout->hasMeshAttribute(MAS_TEXCOORD))
    {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, mVertexLayout->getMeshAttributeStride(), (GLvoid*)mVertexLayout->getMeshAttributeOffset(MAS_TEXCOORD));
    }

    if (mVertexLayout->hasMeshAttribute(MAS_NORMAL))
    {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, mVertexLayout->getMeshAttributeStride(), (GLvoid*)mVertexLayout->getMeshAttributeOffset(MAS_NORMAL));
    }

    if (mVertexLayout->hasMeshAttribute(MAS_TANGENT))
    {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, mVertexLayout->getMeshAttributeStride(), (GLvoid*)mVertexLayout->getMeshAttributeOffset(MAS_TANGENT));
    }

    if (mVertexLayout->hasMeshAttribute(MAS_BLEND_INDICES))
    {
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_INT, GL_FALSE, mVertexLayout->getMeshAttributeStride(), (GLvoid*)mVertexLayout->getMeshAttributeOffset(MAS_BLEND_INDICES));

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mVertexLayout->getMeshAttributeStride(), (GLvoid*)mVertexLayout->getMeshAttributeOffset(MAS_BLEND_WEIGHTS));
    }

    glDrawArrays(gfxPrimitiveModeToGLPrimitiveMode(primitiveMode), first, count);
}

void GfxCommandBuffer::drawIndexed(const GfxPrimitiveMode& primitiveMode, const int& indexCount)
{
    mProgram->bind();

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer->mHandle);

    if (mVertexLayout->hasMeshAttribute(MAS_POSITION))
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mVertexLayout->getMeshAttributeStride(), (GLvoid*)mVertexLayout->getMeshAttributeOffset(MAS_POSITION));
    }

    if (mVertexLayout->hasMeshAttribute(MAS_TEXCOORD))
    {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, mVertexLayout->getMeshAttributeStride(), (GLvoid*)mVertexLayout->getMeshAttributeOffset(MAS_TEXCOORD));
    }

    if (mVertexLayout->hasMeshAttribute(MAS_NORMAL))
    {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, mVertexLayout->getMeshAttributeStride(), (GLvoid*)mVertexLayout->getMeshAttributeOffset(MAS_NORMAL));
    }

    if (mVertexLayout->hasMeshAttribute(MAS_TANGENT))
    {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, mVertexLayout->getMeshAttributeStride(), (GLvoid*)mVertexLayout->getMeshAttributeOffset(MAS_TANGENT));
    }

    if (mVertexLayout->hasMeshAttribute(MAS_BLEND_INDICES))
    {
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_INT, GL_FALSE, mVertexLayout->getMeshAttributeStride(), (GLvoid*)mVertexLayout->getMeshAttributeOffset(MAS_BLEND_INDICES));

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mVertexLayout->getMeshAttributeStride(), (GLvoid*)mVertexLayout->getMeshAttributeOffset(MAS_BLEND_WEIGHTS));
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer->mHandle);

    glDrawElements(gfxPrimitiveModeToGLPrimitiveMode(primitiveMode), indexCount, GL_UNSIGNED_INT, 0);
}

void GfxCommandBuffer::reset()
{
    mProgram = nullptr;
    mVertexBuffer = nullptr;
    mIndexBuffer = nullptr;
    mVertexLayout = nullptr;
}

EFFECTS_NAMESPACE_END