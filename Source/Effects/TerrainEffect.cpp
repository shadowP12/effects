#include "TerrainEffect.h"
#include "../Core/Utility/FileUtility.h"
#include "../Core/Gfx/GpuProgram.h"
#include "../Core/Gfx/GfxDebug.h"
#include "../Core/Scene/RenderScene.h"
#include "../Math/Geometry.h"
#include "../Core/Renderer/Renderer.h"
#include "../Core/Renderer/Renderable.h"
#include "../Core/RenderResources/Mesh.h"
#include "../Core/RenderResources/Material.h"
#define STB_IMAGE_IMPLEMENTATIO
#include <stb_image.h>

EFFECTS_NAMESPACE_BEGIN

static GLuint quadVAO = 0;
static GLuint quadVBO = 0;
static GLuint canvasFB = 0;
static GLuint canvasMap = 0;
static GLuint brushMap = 0;
static GpuProgram* textureProgram = nullptr;
static GpuProgram* brushProgram = nullptr;
static GpuProgram* terrainProgram = nullptr;
static void renderQuad();
static std::vector<glm::vec3> points;
static GLuint loadTexture(std::string file);
static glm::vec3 getNDCCoord(const float& x, const float& y, const int& width, const int& height);
TerrainEffect::TerrainEffect(int width, int height)
	:BaseEffect(width, height)
{
}

TerrainEffect::~TerrainEffect()
{
	delete textureProgram;
    delete brushProgram;
    delete terrainProgram;
}

void TerrainEffect::prepare()
{
	std::string vs_path = getCurrentPath() + R"(\BuiltinResources\Shaders\texture.vs)";
	std::string vs;
	readFileData(vs_path, vs);
	std::string fs_path = getCurrentPath() + R"(\BuiltinResources\Shaders\texture.fs)";
	std::string fs;
	readFileData(fs_path, fs);
    textureProgram = new GpuProgram(vs, fs);

    vs_path = getCurrentPath() + R"(\BuiltinResources\Shaders\brush.vs)";
    readFileData(vs_path, vs);
    fs_path = getCurrentPath() + R"(\BuiltinResources\Shaders\brush.fs)";
    readFileData(fs_path, fs);
    brushProgram = new GpuProgram(vs, fs);

    vs_path = getCurrentPath() + R"(\BuiltinResources\Shaders\terrain.vs)";
    readFileData(vs_path, vs);
    fs_path = getCurrentPath() + R"(\BuiltinResources\Shaders\terrain.fs)";
    readFileData(fs_path, fs);
    terrainProgram = new GpuProgram(vs, fs);

    glGenTextures(1, &canvasMap);
    glBindTexture(GL_TEXTURE_2D, canvasMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenFramebuffers(1, &canvasFB);
    glBindFramebuffer(GL_FRAMEBUFFER, canvasFB);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, canvasMap, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    brushMap = loadTexture(getCurrentPath() + R"(\BuiltinResources\Textures\brush.png)");
    glBindTexture(GL_TEXTURE_2D, brushMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void TerrainEffect::update(float t)
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
	    if(input->m_mouse_position.x < m_width / 3 && input->m_mouse_position.y > m_height - m_height / 3)
	    {
            glm::vec3 pp = getNDCCoord(input->m_mouse_position.x * 3, (input->m_mouse_position.y * 3) - 2 * m_height, m_width, m_height);
            points.push_back(pp);
            printf("%f   %f  %f\n", pp.x, pp.y, pp.z);
        }

	}
    if (input->m_mouse_button_held[0])
    {

    }
}

void TerrainEffect::render()
{
    glViewport(0, 0, m_width, m_height);
    glBindFramebuffer(GL_FRAMEBUFFER, canvasFB);
    glClearColor(0.8f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < points.size(); ++i)
    {
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(glm::mat4(1.0), points[i]);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 1.0f));
        glUseProgram(brushProgram->getGpuProgram());
        glUniformMatrix4fv(glGetUniformLocation(brushProgram->getGpuProgram(), "u_model"), 1, GL_FALSE, &model[0][0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, brushMap);
        glUniform1i(glGetUniformLocation(brushProgram->getGpuProgram(), "u_texture"), 0);
        renderQuad();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, m_width / 3, m_height / 3);
	glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(textureProgram->getGpuProgram());
    glBindTexture(GL_TEXTURE_2D, canvasMap);
    renderQuad();
//	glm::mat4 model = glm::mat4(1.0);
//	glm::mat4 view = m_context->getCamera()->getViewMatrix();
//	glm::mat4 proj = m_context->getCamera()->getProjectionMatrix(m_width, m_height);
//	glUniformMatrix4fv(glGetUniformLocation(debug_program, "model"), 1, GL_FALSE, &model[0][0]);
//	glUniformMatrix4fv(glGetUniformLocation(debug_program, "view"), 1, GL_FALSE, &view[0][0]);
//	glUniformMatrix4fv(glGetUniformLocation(debug_program, "projection"), 1, GL_FALSE, &proj[0][0]);
}

static void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,};
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

static GLuint loadTexture(std::string file)
{
    int width, height, channels;
    unsigned char * pixels = stbi_load(file.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (!pixels)
    {
        return 0;
    }
    GLuint texture_id = 0;
    if (channels == 4)
    {
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    }
    else if (channels == 3)
    {
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    }
    else
    {
        printf("invalid texture's channel\n");
    }
    stbi_image_free(pixels);
    return texture_id;
}

static glm::vec3 getNDCCoord(const float& x, const float& y, const int& width, const int& height)
{
    glm::vec3 res = glm::vec3((x) / width, (height - y) / height, 0.0f);
    res.x = res.x * 2.0f - 1.0f;
    res.y = res.y * 2.0f - 1.0f;
    //res.z = res.z * 2.0f - 1.0f;
    return res;
}

EFFECTS_NAMESPACE_END