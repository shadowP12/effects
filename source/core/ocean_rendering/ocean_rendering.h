#pragma once
#include "../../utils/base.h"
#include "../../utils/gl_inc.h"
#include "../../utils/gl_utils.h"
#include "../../utils/utils.h"

class OceanRenderer
{
public:
	OceanRenderer();
	~OceanRenderer();
	void set_camera(Camera* camera);
	void prepare();
	void update(float t);
	void render();
private:
	void create_grid(float horizontal_length, float vertical_length, uint32_t rows, uint32_t columns);
	std::complex<float> h(uint32_t n, uint32_t m, float t);
	std::complex<float> h0(int n, int m, std::complex<float> xi);
	std::complex<float> ph(int n, int m);
private:
	Camera* m_camera;
	glm::vec2 m_wind_direction;
	float m_ocean_patch_length;
	float* m_height_data;
	std::vector<glm::vec3> m_mesh_vertices;
	std::vector<glm::vec2> m_mesh_texcoords;
	std::vector<uint32_t> m_mesh_indices;
};
