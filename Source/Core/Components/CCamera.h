#pragma once
#include "../Base.h"
#include "../Scene/Component.h"
#include "../../Math/Math.h"
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
    void setFov(float fov);
    void setNear(float near);
    void setFar(float far);
    void setViewPort(float x, float y, float w, float h);
    void getFov(float fov);
    void getNear(float near);
    void getFar(float far);
    void getViewPort(float x, float y, float w, float h);
protected:
    std::shared_ptr<RenderView> mInternal;
};

EFFECTS_NAMESPACE_END