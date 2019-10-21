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
	
private:
	Scene* m_scene;
	GpuProgram* m_program;	
	GLuint m_program_id;
};

EFFECTS_NAMESPACE_END