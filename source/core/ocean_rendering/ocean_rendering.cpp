#include "ocean_rendering.h"
#include <fftw3.h>

#define N 64
#define A 3e-6f
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

void OceanRenderer::prepare()
{
	//初始化参数
	m_ocean_patch_length = 512.0;
	m_wind_direction = glm::vec2(0.5, 0.5);
	m_wind_direction = glm::normalize(m_wind_direction);
	float g = 9.8;
	float v = 0.5;
	float l = v * v / g;
	m_height_data = new float[N * N];
}

void OceanRenderer::update(float t)
{
	fftw_complex *in, *out;
	fftw_plan plan;

	in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * N);
	out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * N);

	for (int n = 0; n < N; ++n)
	{
		for (int m = 0; m < N; ++m)
		{
			std::complex<float> _h = h(n, m, t);
			in[m * N + n][0] = real(_h);
			in[m * N + n][1] = imag(_h);
		}
	}

	plan = fftw_plan_dft_2d(N, N, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
	fftw_execute(plan);

	for (int n = 0; n < N; ++n)
	{
		for (int m = 0; m < N; ++m) 
		{
			m_height_data[n * N + m] = out[m * N + n][0];
		}
	}

	fftw_destroy_plan(plan);
	fftw_free(in);
	fftw_free(out);
}

void OceanRenderer::render()
{
	
}

std::complex<float> OceanRenderer::h(uint32_t n, uint32_t m, float t)
{
	//_k为k的模
	glm::vec2 k;
	k.x = (n - N / 2) * (2.0 * PI / m_ocean_patch_length);
	k.y = (m - N / 2) * (2.0 * PI / m_ocean_patch_length);
	float _k = glm::length(k);
	float w = sqrt(9.8 * _k);
	//快速验证未使用随机数
	std::complex<float> i(0, t), xi(0.5, 0.5);
	return h0(n, m, xi)*exp(i * w) + conj(h0(-n, -m, xi))*exp(-i * w);
}

std::complex<float> OceanRenderer::h0(int n, int m, std::complex<float> xi)
{
	return sqrt(0.5f) * xi * sqrt(ph(n, m));
}

std::complex<float> OceanRenderer::ph(int n, int m)
{
	glm::vec2 k;
	k.x = (n - N / 2) * (2.0 * PI / m_ocean_patch_length);
	k.y = (m - N / 2) * (2.0 * PI / m_ocean_patch_length);
	float _k = glm::length(k);
	//v为风速
	float v = 0.5;
	float l = v * v / 9.8;
	float _dot = glm::dot(k, m_wind_direction);
	return A * expf(-1.0 / (_k * l * _k * l)) / (_k * _k * _k * _k) * _dot * _dot;
}
