#include "ocean_rendering.h"
#include <fftw3.h>

//频谱的尺寸,必须为2的倍数
#define N 128
//水波高度
#define A 0.00008
//fftw资源
fftw_complex *in_data, *out_data;
fftw_plan plan;

OceanRenderer::OceanRenderer(int width, int height)
	:CoreRenderer(width, height)
{
	m_random_gen.seed(time(NULL));
}

OceanRenderer::~OceanRenderer()
{
	delete[] m_height_data;
	//销毁gl资源
	//销毁fftw资源
	fftw_destroy_plan(plan);
	fftw_free(in_data);
	fftw_free(out_data);
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
	m_ocean_patch_length = 256.0f;
	m_wind_direction = glm::vec2(3.0, 3.0);
	m_wind_direction = glm::normalize(m_wind_direction);
	m_wind_speed = 30.0;
	m_height_data = new float[N * N];

	create_grid(m_ocean_patch_length, m_ocean_patch_length, N, N);

	m_program = create_shader_program("F:/Dev/effects/source/res/shaders/default.vs", "F:/Dev/effects/source/res/shaders/default.fs");
	m_test_map = load_texture("F:/Dev/effects/source/res/textures/test.png");
	glBindTexture(GL_TEXTURE_2D, m_test_map);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenVertexArrays(1, &m_grid_vao);
	glGenBuffers(1, &m_grid_vbo);
	glGenBuffers(1, &m_grid_ibo);

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

	glGenTextures(1, &m_hight_map);
	glBindTexture(GL_TEXTURE_2D, m_hight_map);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//初始化fftw资源
	in_data = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * N);
	out_data = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * N);
	plan = fftw_plan_dft_2d(N, N, in_data, out_data, FFTW_BACKWARD, FFTW_ESTIMATE);
}

void OceanRenderer::update(float t)
{
	//LOGI("%f\n",t);
	for (int n = 0; n < N; ++n)
	{
		for (int m = 0; m < N; ++m)
		{
			std::complex<float> _h = h(n, m, t);
			in_data[m * N + n][0] = std::real(_h);
			in_data[m * N + n][1] = std::imag(_h);
		}
	}

	fftw_execute(plan);

	for (int n = 0; n < N; ++n)
	{
		for (int m = 0; m < N; ++m)
		{
			m_height_data[m * N + n] = out_data[m * N + n][0];
		}
	}
	glBindTexture(GL_TEXTURE_2D, m_hight_map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, N, N, 0, GL_RED, GL_FLOAT, m_height_data);
}

void OceanRenderer::render()
{
	//更新顶点数据
	//glBindBuffer(GL_ARRAY_BUFFER, m_grid_vbo);
	//glBufferData(GL_ARRAY_BUFFER, m_mesh_vertices.size() * sizeof(Vertex), &m_mesh_vertices[0], GL_STATIC_DRAW);
	//绘制
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_program);
	glm::mat4 model = glm::mat4(1.0);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 view = get_camera_view_matrix(m_device->camera);
	glm::mat4 proj = get_camera_projection_matrix(m_device->camera, m_width, m_height);
	glUniformMatrix4fv(glGetUniformLocation(m_program, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_program, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_program, "projection"), 1, GL_FALSE, &proj[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_hight_map);
	glUniform1i(glGetUniformLocation(m_program, "u_hightMap"), 0);
	glBindVertexArray(m_grid_vao);
	glDrawElements(GL_LINES, m_mesh_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

std::complex<float> OceanRenderer::h(uint32_t n, uint32_t m, float t)
{
	//_k为k的模
	glm::vec2 k;
	k.x = (2 * PI * n - PI * N) / m_ocean_patch_length;
	k.y = (2 * PI * m - PI * N) / m_ocean_patch_length;
	float k_length = glm::length(k);
	float g = 9.81f;
	float w = sqrt(g * k_length);
	std::complex<float> i_1(0, t * w);
	std::complex<float> i_2(0, -1.0f * t * w);
	std::complex<float> xi(m_xi(m_random_gen), m_xi(m_random_gen));
	return h0(n, m, xi) * exp(i_1) + std::conj(h0(-n, -m, xi)) * exp(i_2);
}

std::complex<float> OceanRenderer::h0(int n, int m, std::complex<float> xi)
{
	std::complex<float> res = sqrt(0.5f) * xi * sqrt(ph(n, m));
	return res;
}

float OceanRenderer::ph(int n, int m)
{
	//菲利普波普
	glm::vec2 k;
	k.x = (2 * PI * n - PI * N) / m_ocean_patch_length;
	k.y = (2 * PI * m - PI * N) / m_ocean_patch_length;
	float k_length = glm::length(k);
	if (k_length == 0.0f)
	{
		return 0.0f;
	}
	float g = 9.81;
	float l = m_wind_speed * m_wind_speed / 9.81f;
	float _dot = glm::dot(k, m_wind_direction);

	float res = 0.0f;
	res = A * exp(-1.0f / pow(k_length * l, 2)) / (pow(k_length, 4)) * pow(k.x * m_wind_direction.x + k.y * m_wind_direction.y, 2.0f);
	return res;
}
