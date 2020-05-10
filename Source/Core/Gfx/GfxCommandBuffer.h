#pragma once
#include "../Base.h"
#include "GfxCommonTypes.h"
EFFECTS_NAMESPACE_BEGIN

class GfxBuffer;
class GfxTexture;
class GfxFramebuffer;
class GfxProgram;
class MeshDataDescription;

class GfxCommandBuffer
{
public:
    GfxCommandBuffer();
    ~GfxCommandBuffer();
    void setViewport(const int& x, const int& y, const int& w, const int& h);
    void setClearValue(const float& r, const float& g, const float& b, const float& a);
    void setProgram(GfxProgram* program);
    void setVertexBuffer(GfxBuffer* buffer);
    void setIndexBuffer(GfxBuffer* buffer);
    void setVertexLayout(MeshDataDescription* vertexLayout);
    void bindFramebuffer(GfxFramebuffer* fb = nullptr);
    void draw(const GfxPrimitiveMode& primitiveMode, const int& first, const int& count);
    void drawIndexed(const GfxPrimitiveMode& primitiveMode, const int& indexCount);
    void reset();
protected:
    GfxProgram* mProgram;
    GfxBuffer* mVertexBuffer;
    GfxBuffer* mIndexBuffer;
    MeshDataDescription* mVertexLayout;
};

EFFECTS_NAMESPACE_END
