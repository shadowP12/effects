#include "PBREffect.h"
#include "../Core/Utility/FileUtility.h"
#include "../Core/Gfx/GpuProgram.h"
#include "../Core/Scene/RenderScene.h"
#include "../UI/LightWidget.h"
#include "../Core/Gfx/GfxDebug.h"
#include <gearmath/Vector3.h>
#include <gearmath/Vector2.h>

EFFECTS_NAMESPACE_BEGIN

PBREffect::PBREffect(int width, int height)
	:BaseEffect(width, height)
{
}

PBREffect::~PBREffect()
{
	delete m_scene;
	delete m_debug_program;
	delete m_program;
	delete m_light;
	delete m_light_widget;
	delete m_debug_lines;
}

void PBREffect::prepare()
{
	std::string scene_file = getCurrentPath() + "\\BuiltinResources\\Scenes\\test.gltf";
	m_scene = new Scene();
	m_scene->load(scene_file);

	std::string vs;
	std::string vs_path = getCurrentPath() + "\\BuiltinResources\\Shaders\\pbr.vs";
	readFileData(vs_path, vs);

	std::string fs;
	std::string fs_path = getCurrentPath() + "\\BuiltinResources\\Shaders\\pbr.fs";
	readFileData(fs_path, fs);

	m_program = new GpuProgram(vs, fs);

	vs_path = getCurrentPath() + "\\BuiltinResources\\Shaders\\debug.vs";
	readFileData(vs_path, vs);
	fs_path = getCurrentPath() + "\\BuiltinResources\\Shaders\\debug.fs";
	readFileData(fs_path, fs);

	m_debug_program = new GpuProgram(vs, fs);

	for (int i = 0; i < m_scene->m_meshs.size(); i++)
	{
		initMesh(m_scene->m_meshs[i]);
	}

	for (int i = 0; i < m_scene->m_nodes.size(); i++)
	{
		Node& node = m_scene->m_nodes[i];
		m_scene->printNodeInfo(node.node_id);
	}

	m_light = new Light();
	m_light_widget = new LightWidget(m_light);
	m_context->getUISystem()->addWidget(m_light_widget);

	m_debug_lines = new DebugLines();
	
	m_debug_lines->addLine(&glm::vec3(0.0, 0.0, 0.0)[0], &glm::vec3(10.0, 0.0, 0.0)[0], &glm::vec4(1.0, 0.0, 0.0, 0.0)[0]);
	m_debug_lines->addLine(&glm::vec3(0.0, 0.0, 0.0)[0], &glm::vec3(0.0, 10.0, 0.0)[0], &glm::vec4(0.0, 1.0, 0.0, 0.0)[0]);
	m_debug_lines->addLine(&glm::vec3(0.0, 0.0, 0.0)[0], &glm::vec3(0.0, 0.0, 10.0)[0], &glm::vec4(0.0, 0.0, 1.0, 0.0)[0]);

}

void PBREffect::update(float t)
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

void PBREffect::render()
{
	GLuint program = m_program->getGpuProgram();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	for (int i = 0; i < m_scene->m_nodes.size(); i++)
	{
		Node& node = m_scene->m_nodes[i];
		if (node.mesh > -1)
		{
			glm::mat4 model = m_scene->getWorldMatrix(node.node_id);
			glm::mat4 view = m_context->getCamera()->getViewMatrix();
			glm::mat4 proj = m_context->getCamera()->getProjectionMatrix(m_width, m_height);
			glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &proj[0][0]);
			// 灯光参数
			glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);
			glUniform3fv(glGetUniformLocation(program, "u_lightPos"), 1, &m_light->position[0]);
			glUniform3fv(glGetUniformLocation(program, "u_lightColor"), 1, &lightColor[0]);
            glUniform1fv(glGetUniformLocation(program, "u_lightRadius"), 1, &m_light->radius);
            glUniform1fv(glGetUniformLocation(program, "u_lightIntensity"), 1, &m_light->intensity);
			// 材质参数
			glm::vec3 albedo = glm::vec3(0.8f, 0.3f, 0.3f);
            glUniform3fv(glGetUniformLocation(program, "u_albedo"), 1, &albedo[0]);
			drawMesh(m_scene->m_meshs[node.mesh]);
		}
	}

	// 绘制debug线段
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