#pragma once
#include "../Base.h"
#include "Gfx.h"
EFFECTS_NAMESPACE_BEGIN

enum class GfxBufferType
{
    Vertex = 0,
    Index,
};

struct GfxBuffer
{
    GLuint id;
    size_t size;
    GfxBufferType type;
};

void GfxGenBuffer(GfxBuffer* buffer, const GfxBufferType& type);
void GfxAllocBuffer(GfxBuffer* buffer, const size_t& size);
void GfxRealseBuffer(GfxBuffer* buffer);

EFFECTS_NAMESPACE_END