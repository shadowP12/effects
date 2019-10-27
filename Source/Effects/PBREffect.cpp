#include "PBREffect.h"
#include "../Core/Utility/FileUtility.h"
#include "../Core/Gfx/GpuProgram.h"
#include "../Core/Scene/RenderScene.h"
#include "../UI/LightWidget.h"

EFFECTS_NAMESPACE_BEGIN

PBREffect::PBREffect(int width, int height)
	:BaseEffect(width, height)
{
}

PBREffect::~PBREffect()
{
	delete m_scene;
	delete m_program;
	delete m_light;
	delete m_light_widget;
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
			glm::vec3 light_dir = glm::vec3(0.5, 0.5, 1.0);
			glm::vec3 light_color = glm::vec3(1.0, 1.0, 1.0);
			glUniform3fv(glGetUniformLocation(program, "u_light_dir"), 1, &m_light->direction[0]);
			glUniform3fv(glGetUniformLocation(program, "u_light_color"), 1, &light_color[0]);
			// 材质参数

			drawMesh(m_scene->m_meshs[node.mesh]);
		}
	}
}

EFFECTS_NAMESPACE_END