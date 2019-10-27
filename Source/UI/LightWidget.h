#pragma once
#include "../Core/Base.h"
#include "UISystem.h"
EFFECTS_NAMESPACE_BEGIN

class Light;

class LightWidget : public UIWidget
{
public:
	LightWidget(Light* light);
	~LightWidget();
	virtual void draw();
private:
	Light* m_light;
};

EFFECTS_NAMESPACE_END