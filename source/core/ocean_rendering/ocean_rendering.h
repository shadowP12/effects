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
	void update();
	void render();
private:
	float phillips_spectrum(float a, float l, glm::vec2 k, glm::vec2 wind_direction);
	void create_grid(float horizontal_length, float vertical_length, uint32_t rows, uint32_t columns);
private:
	Camera* m_camera;
	glm::vec2 m_wind_direction;
	uint32_t m_number;
	float m_ocean_patch_length;
	float* m_height_data;
	std::vector<glm::vec3> m_mesh_vertices;
	std::vector<glm::vec2> m_mesh_texcoords;
	std::vector<uint32_t> m_mesh_indices;
};
