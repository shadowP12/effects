#pragma once

#include "Core/Base.h"
#include "Core/Gfx/Gfx.h"
#include "Scene/CommonTool.h"
#include <vector>
EFFECTS_NAMESPACE_BEGIN

class RenderView;
class Renderable;
class GfxProgram;

enum class EffectType {
    DEFAULT = 0,
    SEA = 1,
    MAX_EFFECT_TYPES
};

class BaseEffect
{
public:
	BaseEffect() {}
	virtual ~BaseEffect() {}
	virtual void render(std::vector<RenderView*>, std::vector<Renderable*>) = 0;
protected:
};

EFFECTS_NAMESPACE_END
