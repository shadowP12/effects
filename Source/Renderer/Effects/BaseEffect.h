#pragma once

#include "Core/Base.h"
#include "Core/Gfx/Gfx.h"
#include "Scene/CommonTool.h"
#include <vector>
EFFECTS_NAMESPACE_BEGIN

class RenderView;
class Renderable;

enum class EffectType {
    DEFAULT = 0,
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
