#pragma once
#include "Core/Base.h"
#include "Scene/Component.h"
#include "Math/Math.h"
EFFECTS_NAMESPACE_BEGIN
class SceneObject;
class RenderView;
class CCamera : public Component
{
public:
    CCamera(std::shared_ptr<SceneObject> so);
    ~CCamera();
    virtual void update();
    static ComponentType getClassType(){return ComponentType::Camera;}
    virtual ComponentType getType(){return mType;}
    void setFov(const float& fov);
    void setNear(const float& near);
    void setFar(const float& far);
    void setViewPort(const float& x, const float& y, const float& w, const float& h);
    float getFov();
    float getNear();
    float getFar();
    glm::vec4 getViewPort();
protected:
    std::shared_ptr<RenderView> mInternal;
};

EFFECTS_NAMESPACE_END