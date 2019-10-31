#pragma once
#include "../Core/Base.h"
#include "BaseEffect.h"

EFFECTS_NAMESPACE_BEGIN


class DebugEffect : public BaseEffect
{
public:
	DebugEffect(int width, int height);
	~DebugEffect();
	virtual void prepare();
	virtual void update(float t);
	virtual void render();
private:

};

EFFECTS_NAMESPACE_END