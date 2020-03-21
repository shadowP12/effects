#pragma once
#include "../Core/Base.h"
#include "BaseEffect.h"
#include <vector>

EFFECTS_NAMESPACE_BEGIN
namespace temp
{
    class Mesh;
};
class GpuProgram;
class DebugLines;

class TerrainEffect : public BaseEffect
{
public:
    TerrainEffect(int width, int height);
	~TerrainEffect();
	virtual void prepare();
	virtual void update(float t);
	virtual void render();
private:
};

EFFECTS_NAMESPACE_END