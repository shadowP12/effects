#include "ocean_rendering.h"
#include <fftw3.h>

#define N 64
#define A 3e-6f
OceanRenderer::OceanRenderer(int width, int height)
{
	resize(width, height);
}

OceanRenderer::~OceanRenderer()
{
	delete[] m_height_data;
}

void OceanRenderer::resize(int width, int height)
{
	m_width = width;
	m_height = height;
	glViewport(0, 0, width, height);
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
		glm::vec3 pos;
		pos.x = horizontal_length * (x - 0.5f);
		pos.y = vertical_length * (y - 0.5f);
		pos.z = 0.0f;
		glm::vec2 uv;
		uv.x = x;
		uv.y = y;
		Vertex vertex;
		vertex.pos = pos;
		vertex.uv = uv;
		m_mesh_vertices.push_back(vertex);
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

	create_grid(m_ocean_patch_length, m_ocean_patch_length, N, N);

	m_program = create_shader_program("F:/Dev/effects/source/res/shaders/default.vs", "F:/Dev/effects/source/res/shaders/default.fs");

	glGenVertexArrays(1, &m_grid_vao);
	glGenBuffers(1, &m_grid_vbo);
	glGenBuffers(1, &m_grid_vbo);

	glBindVertexArray(m_grid_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_grid_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_mesh_vertices.size() * sizeof(Vertex), &m_mesh_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_grid_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_mesh_indices.size() * sizeof(uint32_t), &m_mesh_indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glBindVertexArray(0);
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
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(m_grid_vao);
	glDrawElements(GL_LINES, m_mesh_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
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
