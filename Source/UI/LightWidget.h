#pragma once
#include "../Core/Base.h"
#include "../Math/Math.h"
#include "UISystem.h"
EFFECTS_NAMESPACE_BEGIN

class LightWidget : public UIWidget
{
public:
	LightWidget();
	~LightWidget();
	virtual void draw();
public:
    // camera
    float mAperture;
    float mShutterSpeed;
    float mSensitivity;

    // main lit
    glm::vec3 mMainLitDir;
    glm::vec4 mMainLitColorIntensity;

    // point lit 0
    glm::vec3 mPointLitPos0;
    float mPointLitRadius0;
    float mPointLitColorIntensity0;

    // point lit 1
    glm::vec3 mPointLitPos1;
    float mPointLitRadius1;
    float mPointLitColorIntensity1;
};

EFFECTS_NAMESPACE_END