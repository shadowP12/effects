#pragma once
#include "Core/Base.h"
#include "Math/Math.h"
EFFECTS_NAMESPACE_BEGIN
class SceneNode;
class CameraController {
public:
    CameraController();
    ~CameraController();
    void setCamera(SceneNode* cam);
    void update(float dt);
private:
    SceneNode* mCameraNode = nullptr;
    glm::vec2 mLastMousePosition;
    float mPitch;
    float mYaw;
    float mRoll;
};

EFFECTS_NAMESPACE_END

