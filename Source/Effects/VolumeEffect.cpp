#include "VolumeEffect.h"
#include "../Core/Utility/FileUtility.h"
#include "../Core/Gfx/GpuProgram.h"
#include "../Core/Gfx/GfxDebug.h"
#include "../Core/Scene/RenderScene.h"
#include "../Math/Geometry.h"
#include "../Core/Renderer/Renderer.h"
#include "../Core/Renderer/Renderable.h"
#include "../Core/RenderResources/Mesh.h"
#include "../Core/RenderResources/Material.h"

EFFECTS_NAMESPACE_BEGIN

VolumeEffect::VolumeEffect(int width, int height)
	:BaseEffect(width, height)
{
}
VolumeEffect::~VolumeEffect()
{
	delete m_debug_program;
	delete m_debug_lines;
	temp::releaseMesh(m_plane_mesh);
}

void VolumeEffect::prepare()
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

	m_plane_mesh = new temp::Mesh();
	preparePlaneMesh();
	temp::initMesh(m_plane_mesh);
}

void VolumeEffect::update(float t)
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
	}
}

void VolumeEffect::render()
{

}

void VolumeEffect::preparePlaneMesh()
{
	temp::Mesh::Vertex v0;
    temp::Mesh::Vertex v1;
    temp::Mesh::Vertex v2;
    temp::Mesh::Vertex v3;
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