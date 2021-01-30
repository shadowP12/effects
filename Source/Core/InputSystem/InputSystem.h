#pragma once
#include "Core/Base.h"
#include "Core/Utility/Event.h"
#include "Core/Utility/Module.h"
#include "Math/Math.h"

EFFECTS_NAMESPACE_BEGIN

class InputSystem : public Module<InputSystem> {
public:
    InputSystem();
    ~InputSystem();
    void reset();
    float getMouseScrollWheel() { return mMouseScrollWheel; }
    bool getMouseButtonHeld(uint8_t i) { return mMouseButtonHeld[i]; }
    bool getMouseButtonDown(uint8_t i) { return mMouseButtonDown[i]; }
    bool getMouseButtonUp(uint8_t i) { return mMouseButtonUp[i]; }
    glm::vec2 getMousePosition() { return mMousePosition; }
    void onMousePosition(float x, float y);
    void onMouseButton(int button, int action);
    void onMouseScroll(float offset);
private:
    float mMouseScrollWheel;
    bool mMouseButtonHeld[3];
    bool mMouseButtonDown[3];
    bool mMouseButtonUp[3];
    glm::vec2 mMousePosition;
};

EFFECTS_NAMESPACE_END