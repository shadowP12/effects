#pragma once
#include "../../utils/base.h"
#include "../../utils/gl_inc.h"
#include "../../utils/gl_utils.h"
#include "../../utils/utils.h"

class OceanRenderer
{
public:
	OceanRenderer(int width, int height);
	~OceanRenderer();
	void set_camera(Camera* camera);
	void prepare();
	void update(float t);
	void render();
	void resize(int width, int height);
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec2 uv;
	};
private:
	void create_grid(float horizontal_length, float vertical_length, uint32_t rows, uint32_t columns);
	std::complex<float> h(uint32_t n, uint32_t m, float t);
	std::complex<float> h0(int n, int m, std::complex<float> xi);
	std::complex<float> ph(int n, int m);
private:
	Camera* m_camera;
	int m_width;
	int m_height;
	glm::vec2 m_wind_direction;
	float m_ocean_patch_length;
	float* m_height_data;
	std::vector<Vertex> m_mesh_vertices;
	std::vector<uint32_t> m_mesh_indices;
	GLuint m_program;
	GLuint m_grid_vao;
	GLuint m_grid_vbo;
	GLuint m_grid_ibo;
	GLuint m_hight_map;
	std::default_random_engine m_random_gen;
	std::normal_distribution<> m_xi{ 0,1 };
};
