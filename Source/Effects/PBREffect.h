#pragma once
#include "../Core/Base.h"
#include "BaseEffect.h"

EFFECTS_NAMESPACE_BEGIN

class LightWidget;
struct Light;

class PBREffect : public BaseEffect
{
public:
	PBREffect(int width, int height);
	~PBREffect();
	void prepare() override ;
	void update(float t) override ;
	void render() override;

private:
	Light* m_light;
	LightWidget* m_light_widget;
};

EFFECTS_NAMESPACE_END