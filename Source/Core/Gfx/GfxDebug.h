#pragma once
#include "../Base.h"
#include "Gfx.h"
#include <map>
#include <string>
EFFECTS_NAMESPACE_BEGIN

//Debug Line
#define MAX_GL_LINES 2048

class DebugLines
{
public:
	DebugLines();
	~DebugLines();
	void draw();
	void addLine(float* start_xyz, float* end_xyz, float* colour_rgba);
private:
	GLuint m_vao, m_vbo;
	uint32_t m_lines_count = 0;//当前需要绘制线的数量
};

EFFECTS_NAMESPACE_END
