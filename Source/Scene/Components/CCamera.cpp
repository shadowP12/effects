#include "CCamera.h"
#include "Scene/Scene.h"
#include "Renderer/RenderView.h"
EFFECTS_NAMESPACE_BEGIN

CCamera::CCamera(std::shared_ptr<SceneObject> so)
    :Component(so)
{
    mType = ComponentType::Camera;
    mInternal = std::make_shared<RenderView>();
    mInternal->initialize();
    mInternal->setFov(45.0f);
    mInternal->setFar(0.1f);
    mInternal->setFar(1000.0f);
    mInternal->setViewPort(0.0f, 0.0f, 800.0f, 600.0f);
}

CCamera::~CCamera()
{
}

void CCamera::update()
{
    mInternal->setTransform(mSceneObject->getLocalMatrix());
}

void CCamera::setFov(const float& fov)
{
    mInternal->setFov(fov);
}

void CCamera::setNear(const float& near)
{
    mInternal->setNear(near);
}

void CCamera::setFar(const float& far)
{
    mInternal->setFar(far);
}

void CCamera::setViewPort(const float& x, const float& y, const float& w, const float& h)
{
    mInternal->setViewPort(x, y, w, h);
}

float CCamera::getFov()
{
    return mInternal->getFov();
}

float CCamera::getNear()
{
    return mInternal->getNear();
}

float CCamera::getFar()
{
    return mInternal->getFar();
}

glm::vec4 CCamera::getViewPort()
{
    return mInternal->getViewPort();
}

EFFECTS_NAMESPACE_END
