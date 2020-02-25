#include "GfxResources.h"

EFFECTS_NAMESPACE_BEGIN

void GfxGenBuffer(GfxBuffer* buffer, const GfxBufferType& type)
{
    buffer->type = type;
    glGenBuffers(1, &buffer->id);
}

void GfxAllocBuffer(GfxBuffer* buffer, const size_t& size)
{
    // todo:
}

void GfxRealseBuffer(GfxBuffer* buffer)
{
    // todo
}

EFFECTS_NAMESPACE_END
