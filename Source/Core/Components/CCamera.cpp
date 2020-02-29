#include "CCamera.h"
#include "../Scene/Scene.h"
#include "../Renderer/RenderView.h"
EFFECTS_NAMESPACE_BEGIN

CCamera::CCamera(std::shared_ptr<SceneObject> so)
    :Component(so)
{
    mType = ComponentType::Camera;
    mFov = 45.0f;
    mNear = 0.1f;
    mFar = 1000.0f;
    mViewPort = glm::vec4(0.0f, 0.0f, 600.0f, 800.0f);
    mInternal = std::make_shared<RenderView>();
    mInternal->initialize();

}

CCamera::~CCamera()
{
}

void CCamera::update()
{
}

void CCamera::setFov(float fov)
{
    mFov = fov;
}

void CCamera::setNear(float near)
{
    mNear = near;
}

void CCamera::setFar(float far)
{
    mFar = far;
}

void CCamera::setViewPort(float x, float y, float w, float h)
{
    mViewPort = glm::vec4(x, y, w, h);
}

EFFECTS_NAMESPACE_END
