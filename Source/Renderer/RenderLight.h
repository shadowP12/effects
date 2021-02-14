#pragma once

#include "Core/Base.h"
#include "Math/Math.h"
#include <memory>
EFFECTS_NAMESPACE_BEGIN

enum class RenderLightType {
    DIRECT_LIGHT = 0,
    POINT_LIGHT
};
class CLight;
class RenderLight
{
public:
    RenderLight(CLight* clight);
    virtual ~RenderLight();
    RenderLightType getType();
    const glm::vec4& getDirection();
    const glm::vec4& getLightColorAndIntensity();
    const glm::vec4& getLightPositionAndInvRadius();
protected:
    CLight* mCLight;
};

EFFECTS_NAMESPACE_END