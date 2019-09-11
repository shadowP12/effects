#include "gl_utils.h"
#include "file_utils.h"

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
