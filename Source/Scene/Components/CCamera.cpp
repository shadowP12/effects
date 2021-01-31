#include "CCamera.h"
#include "Scene/Scene.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderView.h"
EFFECTS_NAMESPACE_BEGIN

CCamera::CCamera(SceneNode* node)
    :Component(node) {
    mInternal = Renderer::instance().addRenderView();
}

CCamera::~CCamera() {
}

void CCamera::initialized() {
    mInternal->initialize();
}

void CCamera::onNodeDirty() {
    mInternal->setTransform(mNode->getLocalMatrix());
}

void CCamera::setFov(const float& fov) {
    mInternal->setFov(fov);
}

void CCamera::setNear(const float& near) {
    mInternal->setNear(near);
}

void CCamera::setFar(const float& far) {
    mInternal->setFar(far);
}

void CCamera::setViewPort(const float& x, const float& y, const float& w, const float& h) {
    mInternal->setViewPort(x, y, w, h);
}

float CCamera::getFov() {
    return mInternal->getFov();
}

float CCamera::getNear() {
    return mInternal->getNear();
}

float CCamera::getFar() {
    return mInternal->getFar();
}

glm::vec4 CCamera::getViewPort() {
    return mInternal->getViewPort();
}

EFFECTS_NAMESPACE_END
