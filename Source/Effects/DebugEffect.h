#pragma once
#include "../Core/Base.h"
#include "BaseEffect.h"

EFFECTS_NAMESPACE_BEGIN

class GpuProgram;
class DebugLines;

class DebugEffect : public BaseEffect
{
public:
	DebugEffect(int width, int height);
	~DebugEffect();
	virtual void prepare();
	virtual void update(float t);
	virtual void render();
private:
	GpuProgram* m_debug_program;
	DebugLines* m_debug_lines;
};

EFFECTS_NAMESPACE_END