#include "pbr_rendering.h"

PBRRenderer::PBRRenderer(int width, int height)
	:CoreRenderer(width, height)
{

}
PBRRenderer::~PBRRenderer()
{
	delete_gltf_scene(m_scene);
	glDeleteProgram(m_program);
}

void PBRRenderer::prepare()
{
	m_scene = load_gltf_scene("F:/Dev/effects/source/res/scenes/test.gltf");
	m_program = create_shader_program("F:/Dev/effects/source/res/shaders/pbr.vs", "F:/Dev/effects/source/res/shaders/pbr.fs");
	for (int i = 0; i < m_scene->meshs.size(); i++)
	{
		init_gltf_mesh(m_scene->meshs[i]);
	}
}

void PBRRenderer::update(float t)
{
}

void PBRRenderer::render()
{
	glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(m_program);

	for (auto& entry : m_scene->nodes)
	{
		if (entry.second.mesh > -1)
		{
			glm::mat4 model = get_world_matrix(m_scene, entry.second.node_id);
			glm::mat4 view = get_camera_view_matrix(m_device->camera);
			glm::mat4 proj = get_camera_projection_matrix(m_device->camera, m_width, m_height);
			glUniformMatrix4fv(glGetUniformLocation(m_program, "model"), 1, GL_FALSE, &model[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(m_program, "view"), 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(m_program, "projection"), 1, GL_FALSE, &proj[0][0]);
			draw_gltf_mesh(m_scene->meshs[entry.second.mesh], 1);
		}
	}
}