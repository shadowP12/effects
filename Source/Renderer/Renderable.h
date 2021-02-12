#pragma once

#include "Core/Base.h"
#include "Math/Math.h"
#include <memory>
EFFECTS_NAMESPACE_BEGIN

enum class RenderableType {
    NONE,
    MESH,
    SEA
};

class Mesh;
class Material;

class Renderable
{
public:
    Renderable();
    virtual ~Renderable();
    virtual void initialize() = 0;
    RenderableType getType() { return mType; }
    void setType(RenderableType type) { mType = type; }
protected:
    RenderableType mType;
};

EFFECTS_NAMESPACE_END