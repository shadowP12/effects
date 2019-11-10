#include "DebugEffect.h"
#include "../Core/Utility/FileUtility.h"
#include "../Core/Gfx/GpuProgram.h"
#include "../Core/Gfx/GfxDebug.h"
EFFECTS_NAMESPACE_BEGIN

DebugEffect::DebugEffect(int width, int height)
	:BaseEffect(width, height)
{
}
DebugEffect::~DebugEffect()
{
	delete m_debug_program;
	delete m_debug_lines;
}

void DebugEffect::prepare()
{
	std::string vs_path = getCurrentPath() + "\\BuiltinResources\\Shaders\\debug.vs";
	std::string vs;
	readFileData(vs_path, vs);
	std::string fs_path = getCurrentPath() + "\\BuiltinResources\\Shaders\\debug.fs";
	std::string fs;
	readFileData(fs_path, fs);
	m_debug_program = new GpuProgram(vs, fs);

	m_debug_lines = new DebugLines();
	m_debug_lines->addLine(&glm::vec3(0.0, 0.0, 0.0)[0], &glm::vec3(10.0, 0.0, 0.0)[0], &glm::vec4(1.0, 0.0, 0.0, 0.0)[0]);
	m_debug_lines->addLine(&glm::vec3(0.0, 0.0, 0.0)[0], &glm::vec3(0.0, 10.0, 0.0)[0], &glm::vec4(0.0, 1.0, 0.0, 0.0)[0]);
	m_debug_lines->addLine(&glm::vec3(0.0, 0.0, 0.0)[0], &glm::vec3(0.0, 0.0, 10.0)[0], &glm::vec4(0.0, 0.0, 1.0, 0.0)[0]);
}

void DebugEffect::update(float t)
{
	Input* input = m_context->getInput();
	Camera* camera = m_context->getCamera();
	if (input->m_mouse_button_down[1])
	{
		input->m_mouse_previou_position = input->m_mouse_position;
	}
	if (input->m_mouse_button_held[1])
	{
		camera->Rotate(input->m_mouse_position - input->m_mouse_previou_position);
		input->m_mouse_previou_position = input->m_mouse_position;
	}
	camera->Move(input->m_mouse_scroll_wheel * 5.0);
}

void DebugEffect::render()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// »æÖÆdebugÏß¶Î
	GLuint debug_program = m_debug_program->getGpuProgram();
	glUseProgram(debug_program);
	glm::mat4 model = glm::mat4(1.0);
	glm::mat4 view = m_context->getCamera()->getViewMatrix();
	glm::mat4 proj = m_context->getCamera()->getProjectionMatrix(m_width, m_height);
	glUniformMatrix4fv(glGetUniformLocation(debug_program, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(debug_program, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(debug_program, "projection"), 1, GL_FALSE, &proj[0][0]);
	m_debug_lines->draw();
}

EFFECTS_NAMESPACE_END