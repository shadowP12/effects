#include "Scene/Components/CLight.h"
#include "Scene/Scene.h"
#include "Renderer/Renderer.h"

EFFECTS_NAMESPACE_BEGIN

CLight::CLight(SceneNode* node)
    :Component(node) {
    mInternal = new RenderLight(this);
    Renderer::instance().addRenderLight(mInternal);
    mDirection = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
    mLightColorAndIntensity = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    mLightPositionAndInvRadius = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

CLight::~CLight() {
}

void CLight::initialized() {
}

void CLight::onNodeDirty() {
    glm::vec3 front = getAxisZ(mNode->getWorldMatrix());
    mDirection.x = front.x;
    mDirection.y = front.y;
    mDirection.z = front.z;

    glm::vec3 position = getTranslate(mNode->getWorldMatrix());
    mLightPositionAndInvRadius.x = position.x;
    mLightPositionAndInvRadius.y = position.y;
    mLightPositionAndInvRadius.z = position.z;
}

RenderLightType CLight::getLightType() {
    return mType;
}

void CLight::setLightType(RenderLightType type) {
    mType = type;
    Renderer::instance().setLightDirty();
}

const glm::vec4& CLight::getLightDirection() {
    return mDirection;
}

void CLight::setLightColorAndIntensity(const glm::vec4& colorAndIntensity) {
    mLightColorAndIntensity = colorAndIntensity;
}

const glm::vec4& CLight::getLightColorAndIntensity() {
    return mLightColorAndIntensity;
}

void CLight::setLightRadius(float radius) {
    mLightPositionAndInvRadius.w = radius;
}

const glm::vec4& CLight::getLightPositionAndInvRadius() {
    return mLightPositionAndInvRadius;
}

EFFECTS_NAMESPACE_END