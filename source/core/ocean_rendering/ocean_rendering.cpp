#include "ocean_rendering.h"

OceanRenderer::OceanRenderer()
{
}

OceanRenderer::~OceanRenderer()
{
	delete[] m_height_data;
}

void OceanRenderer::set_camera(Camera* camera)
{
	m_camera = camera;
}

void OceanRenderer::create_grid(float horizontal_length, float vertical_length, uint32_t rows, uint32_t columns)
{
	uint32_t number_vertices = (rows + 1) * (columns + 1);
	uint32_t number_indices = rows * columns * 6;
	
	float x, y;
	for (uint32_t i = 0; i < number_vertices; i++)
	{
		x = (float)(i % (columns + 1)) / (float)columns;
		y = 1.0 - (float)(i / (columns + 1)) / (float)rows;
		glm::vec3 vertex;
		vertex.x = horizontal_length * (x - 0.5f);
		vertex.y = vertical_length * (y - 0.5f);
		vertex.z = 0.0f;
		glm::vec2 uv;
		uv.x = x;
		uv.y = y;
		m_mesh_vertices.push_back(vertex);
		m_mesh_texcoords.push_back(uv);
	}

	uint32_t current_column, current_row;
	for (uint32_t i = 0; i < rows * columns; i++)
	{
		current_column = i % columns;
		current_row = i / columns;
		m_mesh_indices.push_back(current_column + current_row * (columns + 1));
		m_mesh_indices.push_back(current_column + (current_row + 1) * (columns + 1));
		m_mesh_indices.push_back((current_column + 1) + (current_row + 1) * (columns + 1));
		m_mesh_indices.push_back((current_column + 1) + (current_row + 1) * (columns + 1));
		m_mesh_indices.push_back((current_column + 1) + current_row * (columns + 1));
		m_mesh_indices.push_back(current_column + current_row * (columns + 1));
	}
}

float OceanRenderer::phillips_spectrum(float a, float l, glm::vec2 k, glm::vec2 wind_direction)
{
	//菲利普频谱(phillips spectrum)
	//l= v * v/ g, v:风速, g:重力加速度
	float _k = glm::length(k);

	if (l == 0.0 || _k == 0.0)
	{
		return 0.0;
	}
	float _dot = glm::dot(k, wind_direction);
	return a * expf(-1.0 / (_k * l * _k * l)) / (_k * _k * _k * _k) * _dot * _dot;
}

void OceanRenderer::prepare()
{
	//初始化参数
	m_number = 64;
	m_ocean_patch_length = 512.0;
	m_wind_direction = glm::vec2(0.5, 0.5);
	m_wind_direction = glm::normalize(m_wind_direction);
	float g = 9.8;
	float v = 0.5;
	float l = v * v / g;
	m_height_data = new float[m_number * m_number * 2 * sizeof(float)];

	//利用phillips频谱计算出初始波形
	glm::vec2 k;
	float phillips_spectrum_value;
	for (uint32_t i = 0; i < m_number; i++)
	{
		k.y = ((float)i - (float)m_number / 2.0) * (2.0 * PI / m_ocean_patch_length);
		for (uint32_t j = 0; j < m_number; j++)
		{
			k.x = ((float)j - (float)m_number / 2.0f) * (2.0f * PI / m_ocean_patch_length);
			phillips_spectrum_value = phillips_spectrum(1.0, l, k, m_wind_direction);
			//todo:需要添加随机波普
			m_height_data[i * 2 * m_number + j * 2 + 0] = 1.0f / sqrtf(2.0f) * 0.5 * phillips_spectrum_value;
			m_height_data[i * 2 * m_number + j * 2 + 1] = 1.0f / sqrtf(2.0f) * 0.3 * phillips_spectrum_value;
		}
	}

}

void OceanRenderer::update()
{

}

void OceanRenderer::render()
{
	
}
