#pragma once
#include "../Core/Base.h"
#include "BaseEffect.h"

EFFECTS_NAMESPACE_BEGIN

namespace temp
{
    class Scene;
};

class GpuProgram;
class LightWidget;
class DebugLines;
struct Light;

class ShadowEffect : public BaseEffect
{
public:
	ShadowEffect(int width, int height);
	~ShadowEffect();
	virtual void prepare();
	virtual void update(float t);
	virtual void render();
private:
	
private:
	temp::Scene* m_scene;
	GpuProgram* m_program;
	Light* m_light;
};

EFFECTS_NAMESPACE_END