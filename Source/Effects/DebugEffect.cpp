#include "DebugEffect.h"
#include "../Core/Utility/FileUtility.h"
#include "../Core/Gfx/GpuProgram.h"
#include "../Core/Gfx/GfxDebug.h"
#include "../Core/Scene/RenderScene.h"
#include "../Math/Geometry.h"

EFFECTS_NAMESPACE_BEGIN

DebugEffect::DebugEffect(int width, int height)
	:BaseEffect(width, height)
{
}
DebugEffect::~DebugEffect()
{
	delete m_debug_program;
	delete m_debug_lines;
	releaseMesh(m_plane_mesh);
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

	m_plane_mesh = new Mesh();
	preparePlaneMesh();
	initMesh(m_plane_mesh);
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

	if (input->m_mouse_button_down[0])
	{
		input->m_mouse_previou_position = input->m_mouse_position;
		glm::vec2 mouse_pos = input->m_mouse_position;
		// screen to ndc
		glm::vec4 screen = glm::vec4((mouse_pos.x) / m_width, ((m_height - mouse_pos.y)) / m_height, 0.0f, 1.0f);

		// to [-1 , 1]
		screen.x = screen.x * 2.0f - 1.0f;
		screen.y = screen.y * 2.0f - 1.0f;
		screen.z = screen.z * 2.0f - 1.0f;

		printf("%f   %f\n", screen.x, screen.y);

	}
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

	drawMesh(m_plane_mesh);
}

void DebugEffect::preparePlaneMesh()
{
	Mesh::Vertex v0;
	Mesh::Vertex v1;
	Mesh::Vertex v2;
	Mesh::Vertex v3;
	v0.position = glm::vec3(-1.0f, 1.0f, 0.0f);
	v0.normal = glm::vec3(1.0f, 1.0f, 1.0f);
	v0.uv = glm::vec2(0.0f, 0.0f);
	v1.position = glm::vec3(-1.0f, -1.0f, 0.0f);
	v1.normal = glm::vec3(1.0f, 1.0f, 1.0f);
	v1.uv = glm::vec2(0.0f, 0.0f);
	v2.position = glm::vec3(1.0f, 1.0f, 0.0f);
	v2.normal = glm::vec3(1.0f, 1.0f, 1.0f);
	v2.uv = glm::vec2(0.0f, 0.0f);
	v3.position = glm::vec3(1.0f, -1.0f, 0.0f);
	v3.normal = glm::vec3(1.0f, 1.0f, 1.0f);
	v3.uv = glm::vec2(0.0f, 0.0f);
	m_plane_mesh->vertices.push_back(v0);
	m_plane_mesh->vertices.push_back(v1);
	m_plane_mesh->vertices.push_back(v2);
	m_plane_mesh->vertices.push_back(v3);

	m_plane_mesh->indices.push_back(0);
	m_plane_mesh->indices.push_back(1);
	m_plane_mesh->indices.push_back(3);
	m_plane_mesh->indices.push_back(0);
	m_plane_mesh->indices.push_back(3);
	m_plane_mesh->indices.push_back(2);
}

EFFECTS_NAMESPACE_END