#include "Renderer/RenderLight.h"
#include "Scene/Components/CLight.h"
EFFECTS_NAMESPACE_BEGIN

RenderLight::RenderLight(CLight* clight) {
    mCLight = clight;
}

RenderLight::~RenderLight() {
}

RenderLightType RenderLight::getType() {
    return mCLight->mType;
}

const glm::vec4& RenderLight::getDirection() {
    return mCLight->mDirection;
}

const glm::vec4& RenderLight::getLightColorAndIntensity() {
    return mCLight->mLightColorAndIntensity;
}

const glm::vec4& RenderLight::getLightPositionAndInvRadius() {
    return mCLight->mLightPositionAndInvRadius;
}

EFFECTS_NAMESPACE_END