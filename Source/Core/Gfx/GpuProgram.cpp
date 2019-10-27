#include "GpuProgram.h"
#include "../Utility/Log.h"
#include <sstream>
#include <fstream>
EFFECTS_NAMESPACE_BEGIN

void checkShaderCompileErrors(GLuint id)
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

void checkProgramCompileErrors(GLuint id)
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

GLuint createGpuProgram(std::string vertex_source, std::string fragment_source)
{
	const char* vertex_source_c = vertex_source.c_str();
	const char* fragment_source_c = fragment_source.c_str();

	GLuint vertex_id, fragment_id;
	vertex_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_id, 1, &vertex_source_c, NULL);
	glCompileShader(vertex_id);
	checkShaderCompileErrors(vertex_id);

	fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_id, 1, &fragment_source_c, NULL);
	glCompileShader(fragment_id);
	checkShaderCompileErrors(fragment_id);

	GLuint program_id;
	program_id = glCreateProgram();
	glAttachShader(program_id, vertex_id);
	glAttachShader(program_id, fragment_id);
	glLinkProgram(program_id);
	checkProgramCompileErrors(program_id);

	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);

	return program_id;
}

GpuProgram::GpuProgram(std::string vertex_source, std::string frament_source)
{
	m_vertex_source = vertex_source;
	m_frament_source = frament_source;
}

GpuProgram::~GpuProgram()
{
	for (auto& var : m_variations)
	{
		glDeleteProgram(var.second);
	}
}

GLuint GpuProgram::getGpuProgram()
{
	GpuProgramDefines defines;
	auto& it = m_variations.find(defines);
	if (it == m_variations.end())
	{
		GLuint res = createVariation(defines);
		m_variations[defines] = res;
		return res;
	}
	else
	{
		return m_variations[defines];
	}
	return 0;
}

GLuint GpuProgram::getGpuProgram(const GpuProgramDefines& defines)
{
	auto& it = m_variations.find(defines);
	if (it == m_variations.end())
	{
		return createVariation(defines);
	}
	else
	{
		return m_variations[defines];
	}
	return 0;
}

GLuint GpuProgram::createVariation(const GpuProgramDefines& defines)
{
	std::stringstream define_block;
	define_block << "#version 330 core" << "\n";
	for (const auto &define : defines)
	{
		define_block << "#define " << define.first << " " << define.second << "\n";
	}
	std::string vertex_source = define_block.str() + m_vertex_source;
	std::string frament_source = define_block.str() + m_frament_source;
	return createGpuProgram(vertex_source, frament_source);
}

EFFECTS_NAMESPACE_END