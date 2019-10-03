#pragma once
#include "base.h"
#include "gl_inc.h"

GLuint create_shader_program(std::string vs, std::string fs);

GLuint load_texture(std::string file);

//Debug Line
#define MAX_GL_LINES 819200
struct DebugLine
{
	GLuint vao, vbo;
	uint32_t lines_count = 0;//当前需要绘制线的数量
};

DebugLine* create_debug_line();

void draw_debug_line(DebugLine* debug_line);

void add_debug_line(DebugLine* debug_line, float* start_xyz, float* end_xyz, float* colour_rgba);

void destroy_debug_line(DebugLine* debug_line);