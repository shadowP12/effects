#pragma once
#include "../Core/Base.h"
#include "BaseEffect.h"

EFFECTS_NAMESPACE_BEGIN

class DebugEffect : public BaseEffect
{
public:
    DebugEffect(int width, int height);
	~DebugEffect();
	void prepare() override ;
	void update(float t) override ;
	void render() override;
};

EFFECTS_NAMESPACE_END