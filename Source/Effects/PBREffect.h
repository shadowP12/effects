#pragma once
#include "../Core/Base.h"
#include "BaseEffect.h"

EFFECTS_NAMESPACE_BEGIN

class Scene;
class GpuProgram;

class PBREffect : public BaseEffect
{
public:
	PBREffect(int width, int height);
	~PBREffect();
	virtual void prepare();
	virtual void update(float t);
	virtual void render();
private:
	Scene* m_scene;
	GpuProgram* m_program;	
};

EFFECTS_NAMESPACE_END