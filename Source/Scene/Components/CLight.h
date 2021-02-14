#pragma once
#include "Core/Base.h"
#include "Scene/Components/Component.h"
#include "Math/Math.h"
#include "Renderer/RenderLight.h"
EFFECTS_NAMESPACE_BEGIN

class SceneNode;
class RenderLight;

class CLight : public Component {
public:
    CLight(SceneNode* node);
    virtual ~CLight();
    static ComponentType getClassType() { return ComponentType::Light; }
    ComponentType getType() override { return ComponentType::Light; }
    void onNodeDirty() override;
    void initialized() override;

    RenderLightType getLightType();
    void setLightType(RenderLightType type);

    const glm::vec4& getLightDirection();

    void setLightColorAndIntensity(const glm::vec4& colorAndIntensity);
    const glm::vec4& getLightColorAndIntensity();

    void setLightRadius(float radius);
    const glm::vec4& getLightPositionAndInvRadius();
protected:
    friend RenderLight;
    RenderLight* mInternal;
    RenderLightType mType = RenderLightType::DIRECT_LIGHT;
    glm::vec4 mDirection;
    glm::vec4 mLightColorAndIntensity;
    glm::vec4 mLightPositionAndInvRadius;

};

EFFECTS_NAMESPACE_END
