#pragma once
#include "../Base.h"
#include "GfxCommonTypes.h"
#include <vector>
#include <functional>
EFFECTS_NAMESPACE_BEGIN

class GfxBuffer;
class GfxTexture;
class GfxFramebuffer;
class GfxProgram;
class MeshDataDescription;

class GfxRenderObj
{
public:
    GfxRenderObj();
    ~GfxRenderObj();
    void setProgram(GfxProgram* program);
    void setVertexBuffer(GfxBuffer* buffer);
    void setIndexBuffer(GfxBuffer* buffer);
    void setVertexLayout(MeshDataDescription* vertexLayout);
    void draw(const GfxPrimitiveMode& primitiveMode, const int& first, const int& count);
    void drawIndexed(const GfxPrimitiveMode& primitiveMode, const int& indexCount);
    void reset();
protected:
    GfxProgram* mCurProgram;
    GfxBuffer* mCurVertexBuffer;
    GfxBuffer* mCurIndexBuffer;
    MeshDataDescription* mCurVertexLayout;
};

EFFECTS_NAMESPACE_END
