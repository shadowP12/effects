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

class DebugEffect : public BaseEffect
{
public:
	DebugEffect(int width, int height);
	~DebugEffect();
	virtual void prepare();
	virtual void update(float t);
	virtual void render();
private:
	void preparePlaneMesh();
private:
	GpuProgram* m_debug_program;
	DebugLines* m_debug_lines;
	temp::Mesh* m_plane_mesh;
	std::vector<glm::vec3> m_hit_points;
};

EFFECTS_NAMESPACE_END