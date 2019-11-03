#include "GfxDebug.h"
#include "../Utility/Log.h"
EFFECTS_NAMESPACE_BEGIN

DebugLines::DebugLines()
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * MAX_GL_LINES * 14, NULL, GL_DYNAMIC_DRAW);

	GLsizei stride = 4 * 7;
	GLintptr offs = 4 * 3;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offs);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}
DebugLines::~DebugLines()
{
	glDeleteBuffers(1, &m_vao);
	glDeleteVertexArrays(1, &m_vbo);
}

void DebugLines::draw()
{
	glBindVertexArray(m_vao);
	glDrawArrays(GL_LINES, 0, 2 * m_lines_count);
	glBindVertexArray(0);
}

void DebugLines::addLine(float* start_xyz, float* end_xyz, float* colour_rgba)
{
	if (m_lines_count >= MAX_GL_LINES)
	{
		LOGE("too many lines");
		return;
	}

	float sd[14];
	sd[0] = start_xyz[0];
	sd[1] = start_xyz[1];
	sd[2] = start_xyz[2];
	sd[3] = colour_rgba[0];
	sd[4] = colour_rgba[1];
	sd[5] = colour_rgba[2];
	sd[6] = colour_rgba[3];
	sd[7] = end_xyz[0];
	sd[8] = end_xyz[1];
	sd[9] = end_xyz[2];
	sd[10] = colour_rgba[0];
	sd[11] = colour_rgba[1];
	sd[12] = colour_rgba[2];
	sd[13] = colour_rgba[3];

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	GLintptr os = sizeof(sd) * m_lines_count;
	GLsizei sz = sizeof(sd);
	glBufferSubData(GL_ARRAY_BUFFER, os, sz, sd);

	m_lines_count++;
}

EFFECTS_NAMESPACE_END