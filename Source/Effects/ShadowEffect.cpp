#include "ShadowEffect.h"
#include "../Core/Utility/FileUtility.h"
#include "../Core/Gfx/GpuProgram.h"
#include "../Core/Scene/RenderScene.h"
#include "../UI/LightWidget.h"
#include "../Core/Gfx/GfxDebug.h"
#include <gearmath/Vector3.h>
#include <gearmath/Vector2.h>

EFFECTS_NAMESPACE_BEGIN

GLuint depthFB = 0;
GLuint depthMap = 0;
GLuint quadVAO = 0;
GLuint quadVBO = 0;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
GpuProgram* mTextureProgram = nullptr;
GpuProgram* mDepthProgram = nullptr;

ShadowEffect::ShadowEffect(int width, int height)
	:BaseEffect(width, height)
{
}

ShadowEffect::~ShadowEffect()
{
	delete m_scene;
	delete m_program;
	delete m_light;
	if (mTextureProgram)
		delete mTextureProgram;
	if (mDepthProgram)
		delete mDepthProgram;
}

void ShadowEffect::prepare()
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

	vs_path = getCurrentPath() + "\\BuiltinResources\\Shaders\\texture.vs";
	readFileData(vs_path, vs);
	fs_path = getCurrentPath() + "\\BuiltinResources\\Shaders\\texture.fs";
	readFileData(fs_path, fs);
	mTextureProgram = new GpuProgram(vs, fs);

	for (int i = 0; i < m_scene->m_meshs.size(); i++)
	{
		initMesh(m_scene->m_meshs[i]);
	}

	m_light = new Light();
	m_light->direction = glm::vec3(0.0f, -1.0f, 0.0f);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenFramebuffers(1, &depthFB);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFB);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	// 禁止颜色渲染
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void ShadowEffect::update(float t)
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

void ShadowEffect::render()
{
	GLuint program = m_program->getGpuProgram();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

	glm::mat4 lightSpaceMatrix = glm::lookAt(glm::vec3(0.0,10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightProjMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 1000.0f);
	
	glViewport(0, 0, m_width/4, m_height/4);
	
	renderQuad();

	glViewport(0, 0, m_width, m_height);
	for (int i = 0; i < m_scene->m_nodes.size(); i++)
	{
		Node& node = m_scene->m_nodes[i];
		if (node.mesh > -1)
		{
			glm::mat4 model = m_scene->getWorldMatrix(node.node_id);
			glm::mat4 view = m_context->getCamera()->getViewMatrix();
			//glm::mat4 proj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 1000.0f);
			//glm::mat4 proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 1000.0f);
			glm::mat4 proj = m_context->getCamera()->getProjectionMatrix(m_width, m_height);
			glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &proj[0][0]);
			// 灯光参数
			glm::vec3 light_dir = glm::vec3(0.5, 0.5, 1.0);
			glm::vec3 light_color = glm::vec3(1.0, 1.0, 1.0);
			glUniform3fv(glGetUniformLocation(program, "u_light_dir"), 1, &m_light->direction[0]);
			glUniform3fv(glGetUniformLocation(program, "u_light_color"), 1
				, &light_color[0]);
			// 材质参数

			drawMesh(m_scene->m_meshs[node.mesh]);
		}
	}
}

EFFECTS_NAMESPACE_END