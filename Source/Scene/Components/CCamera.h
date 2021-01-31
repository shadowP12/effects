#pragma once
#include "Core/Base.h"
#include "Scene/Components/Component.h"
#include "Math/Math.h"
EFFECTS_NAMESPACE_BEGIN
class SceneNode;
class RenderView;
class CCamera : public Component
{
public:
    CCamera(SceneNode* node);
    virtual ~CCamera();
    static ComponentType getClassType() { return ComponentType::Camera; }
    ComponentType getType() override { return mType; }
    void onNodeDirty() override;
    void setFov(const float& fov);
    void setNear(const float& near);
    void setFar(const float& far);
    void setViewPort(const float& x, const float& y, const float& w, const float& h);
    float getFov();
    float getNear();
    float getFar();
    glm::vec4 getViewPort();
protected:
    RenderView* mInternal;
};

EFFECTS_NAMESPACE_END