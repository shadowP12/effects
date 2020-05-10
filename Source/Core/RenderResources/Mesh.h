#pragma once

#include <vector>
#include "../Base.h"
#include "../Gfx/Gfx.h"
EFFECTS_NAMESPACE_BEGIN

class MeshData;

class Mesh
{
public:
    Mesh(MeshData* data);
    ~Mesh();
    void initialize();
    GLuint getVertexBufferArray();
    GLuint getIndexBuffer();

    uint32_t getIndexCount();
    void draw();
private:
    MeshData* mData;
    GLuint mVertexBufferArray;
    GLuint mVertexBuffer;
    GLuint mIndexBuffer;
};

EFFECTS_NAMESPACE_END