#include "GfxRenderer.h"
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

GfxRenderer::GfxRenderer()
{
    reset();
}

GfxRenderer::~GfxRenderer()
{
}

void GfxRenderer::setViewport(const int& x, const int& y, const int& w, const int& h)
{
    glViewport(x, y, w, h);
}

void GfxRenderer::setClearValue(const float& r, const float& g, const float& b, const float& a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GfxRenderer::setProgram(GfxProgram* program)
{
    mCurProgram = program;
}

void GfxRenderer::setVertexBuffer(GfxBuffer* buffer)
{
    mCurVertexBuffer = buffer;
}

void GfxRenderer::setIndexBuffer(GfxBuffer* buffer)
{
    mCurIndexBuffer = buffer;
}

void GfxRenderer::setVertexLayout(MeshDataDescription* vertexLayout)
{
    mCurVertexLayout = vertexLayout;
}

void GfxRenderer::bindFramebuffer(GfxFramebuffer* fb)
{
    if(!fb)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }
    fb->bind();
}

void GfxRenderer::draw(const GfxPrimitiveMode& primitiveMode, const int& first, const int& count)
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

void GfxRenderer::drawIndexed(const GfxPrimitiveMode& primitiveMode, const int& indexCount)
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

void GfxRenderer::reset()
{
    mCurProgram = nullptr;
    mCurVertexBuffer = nullptr;
    mCurIndexBuffer = nullptr;
    mCurVertexLayout = nullptr;
}

EFFECTS_NAMESPACE_END