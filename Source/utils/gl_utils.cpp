#include "gl_utils.h"
#include "file_utils.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

void check_shader_compile_errors(GLuint id)
{
	GLint success;
	GLchar info_log[1024];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 1024, NULL, info_log);
		LOGE("shader compile error : %s \n", info_log);
	}
}

void check_program_compile_errors(GLuint id)
{
	GLint success;
	GLchar info_log[1024];
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 1024, NULL, info_log);
		LOGE("program compile error : %s \n", info_log);
	}
}

GLuint create_shader_program(std::string vs, std::string fs)
{
	std::string vertex_source, fragment_source;
	read_file(vs, vertex_source);
	read_file(fs, fragment_source);
	const char* vertex_source_c = vertex_source.c_str();
	const char* fragment_source_c = fragment_source.c_str();

	GLuint vertex_id, fragment_id;
	vertex_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_id, 1, &vertex_source_c, NULL);
	glCompileShader(vertex_id);
	check_shader_compile_errors(vertex_id);

	fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_id, 1, &fragment_source_c, NULL);
	glCompileShader(fragment_id);
	check_shader_compile_errors(fragment_id);

	GLuint program_id;
	program_id = glCreateProgram();
	glAttachShader(program_id, vertex_id);
	glAttachShader(program_id, fragment_id);
	glLinkProgram(program_id);
	check_program_compile_errors(program_id);

	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);

	return program_id;
}

GLuint load_texture(std::string file)
{
	int width, height, channels;
	unsigned char * pixels = stbi_load(file.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if (!pixels)
	{
		return 0;
	}
	GLuint texture_id = 0;
	if (channels == 4)
	{
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}
	else if (channels == 3)
	{
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	}
	else
	{
		LOGE("invalid texture's channel\n");
	}
	stbi_image_free(pixels);
	return texture_id;
}

DebugLine* create_debug_line()
{
	DebugLine* debug_line = new DebugLine();
	glGenVertexArrays(1, &debug_line->vao);
	glBindVertexArray(debug_line->vao);
	glGenBuffers(1, &debug_line->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, debug_line->vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * MAX_GL_LINES * 14, NULL, GL_DYNAMIC_DRAW);

	GLsizei stride = 4 * 7;
	GLintptr offs = 4 * 3;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offs);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	return debug_line;
}

void draw_debug_line(DebugLine* debug_line)
{
	glBindVertexArray(debug_line->vao);
	glDrawArrays(GL_LINES, 0, 2 * debug_line->lines_count);
	glBindVertexArray(0);
}

void add_debug_line(DebugLine* debug_line, float* start_xyz, float* end_xyz, float* colour_rgba)
{
	if (debug_line->lines_count >= MAX_GL_LINES)
	{
		LOGE("too many gl lines");
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

	glBindBuffer(GL_ARRAY_BUFFER, debug_line->vbo);
	GLintptr os = sizeof(sd) * debug_line->lines_count;
	GLsizei sz = sizeof(sd);
	glBufferSubData(GL_ARRAY_BUFFER, os, sz, sd);

	debug_line->lines_count++;
}

void destroy_debug_line(DebugLine* debug_line)
{
	glDeleteBuffers(1, &debug_line->vao);
	glDeleteVertexArrays(1, &debug_line->vbo);
	delete debug_line;
}
