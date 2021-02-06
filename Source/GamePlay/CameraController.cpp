#include "GamePlay/CameraController.h"
#include "Scene/Scene.h"
#include "Scene/Components/CCamera.h"
#include "Core/InputSystem/InputSystem.h"
#include "Core/Utility/Log.h"
EFFECTS_NAMESPACE_BEGIN

CameraController::CameraController() {
}

CameraController::~CameraController() {
}

void CameraController::setCamera(SceneNode* cam) {
   if (cam->getComponent<CCamera>()) {
       mCameraNode = cam;
       glm::vec3 euler = glm::eulerAngles(cam->getRotation()) * 3.14159f / 180.f;
       mPitch = euler.x;
       mYaw = euler.y;
       mRoll = euler.z;
   }
}

void CameraController::update(float dt) {
    if (!mCameraNode)
        return;

    if (InputSystem::instance().getMouseButtonDown(1)) {
        mLastMousePosition = InputSystem::instance().getMousePosition();

    }
    if (InputSystem::instance().getMouseButtonHeld(1)) {
        glm::vec2 offset = InputSystem::instance().getMousePosition() - mLastMousePosition;
        mYaw -= offset.x * 0.1f;
        mPitch -= offset.y * 0.1f;
        glm::quat rot = glm::quat(glm::vec3(glm::radians(mPitch), glm::radians(mYaw), glm::radians(mRoll)));
        mCameraNode->setRotation(rot);
        mLastMousePosition = InputSystem::instance().getMousePosition();
    }

    if (InputSystem::instance().getMouseScrollWheel() != 0.0f) {
        glm::vec3 cameraPos = mCameraNode->getPosition();
        glm::vec3 cameraFront = mCameraNode->getFrontVector();
        cameraPos += cameraFront * InputSystem::instance().getMouseScrollWheel() * 5.0f * 0.1f;
        mCameraNode->setPosition(cameraPos);
    }

}

EFFECTS_NAMESPACE_END