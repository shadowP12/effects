#include "Core/InputSystem//InputSystem.h"

EFFECTS_NAMESPACE_BEGIN

InputSystem::InputSystem() {
    reset();
}

InputSystem::~InputSystem() {
}

void InputSystem::reset() {
    mMousePosition = glm::vec2(0.0f, 0.0f);
    memset(mMouseButtonUp, 0, sizeof(mMouseButtonUp));
    memset(mMouseButtonDown, 0, sizeof(mMouseButtonDown));
    mMouseScrollWheel = 0.0;
}

void InputSystem::onMousePosition(float x, float y) {
    mMousePosition = glm::vec2(x, y);
}

void InputSystem::onMouseButton(int button, int action) {
    switch (action) {
        case 0:
            mMouseButtonUp[button] = true;
            mMouseButtonHeld[button] = false;
            break;
        case 1:
            mMouseButtonDown[button] = true;
            mMouseButtonHeld[button] = true;
            break;
        default:
            break;
    }
}

void InputSystem::onMouseScroll(float offset) {
    mMouseScrollWheel = offset;
}

EFFECTS_NAMESPACE_END