#pragma once
#include "../Core/Base.h"
#include "BaseEffect.h"

EFFECTS_NAMESPACE_BEGIN

class Scene;
class GpuProgram;
class LightWidget;
class DebugLines;
struct Light;

class PBREffect : public BaseEffect
{
public:
	PBREffect(int width, int height);
	~PBREffect();
	virtual void prepare();
	virtual void update(float t);
	virtual void render();
private:
	
private:
	Scene* m_scene;
	GpuProgram* m_program;
	GpuProgram* m_debug_program;
	Light* m_light;
	LightWidget* m_light_widget;
	DebugLines* m_debug_lines;
};

EFFECTS_NAMESPACE_END