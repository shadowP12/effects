#include "TerrainEffect.h"
#include "../Core/Utility/FileUtility.h"
#include "../Core/Gfx/GpuProgram.h"
#include "../Core/Gfx/GfxDebug.h"
#include "../Math/Geometry.h"
#include "../Core/RenderResources/Mesh.h"
#include "../Importers/TextureImporter.h"
#include "../Core/Renderer/RenderHelper.h"
EFFECTS_NAMESPACE_BEGIN

static GLuint quadVAO = 0;
static GLuint quadVBO = 0;
static GfxTexture* brushTex = nullptr;
static GfxTexture* canvasTex = nullptr;
static GfxFramebuffer* canvasFb = nullptr;
static GfxProgram* textureProgram = nullptr;
static GfxProgram* brushProgram = nullptr;
static GfxProgram* terrainProgram = nullptr;

static void renderQuad();
static std::vector<glm::vec3> points;
static glm::vec3 getNDCCoord(const float& x, const float& y, const int& width, const int& height);
TerrainEffect::TerrainEffect(int width, int height)
	:BaseEffect(width, height)
{
}

TerrainEffect::~TerrainEffect()
{
    delete brushTex;
    delete canvasTex;
    delete canvasFb;
	delete textureProgram;
    delete brushProgram;
    delete terrainProgram;
}

void TerrainEffect::prepare()
{
    textureProgram = loadProgram(getCurrentPath() + R"(\BuiltinResources\Shaders\texture.vs)",
                                 getCurrentPath() + R"(\BuiltinResources\Shaders\texture.fs)");

    brushProgram = loadProgram(getCurrentPath() + R"(\BuiltinResources\Shaders\brush.vs)",
                               getCurrentPath() + R"(\BuiltinResources\Shaders\brush.fs)");

    terrainProgram = loadProgram(getCurrentPath() + R"(\BuiltinResources\Shaders\terrain.vs)",
                               getCurrentPath() + R"(\BuiltinResources\Shaders\terrain.fs)");

    GfxTextureDesc texDesc;
    texDesc.width = m_width;
    texDesc.height = m_height;
    texDesc.componentType = GfxPixelComponentType::BYTE;
    texDesc.format = GfxPixelFormat::RGBA8;
    canvasTex = new GfxTexture(texDesc);

    brushTex = loadTexture(getCurrentPath() + R"(\BuiltinResources\Textures\brush.png)");

    GfxFramebufferDesc fbDesc;
    fbDesc.targets[0] = canvasTex;
    canvasFb = new GfxFramebuffer(fbDesc);
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
    canvasFb->bind();
    glClearColor(0.8f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < points.size(); ++i)
    {
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(glm::mat4(1.0), points[i]);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 1.0f));
        brushProgram->setMat4("u_model", &model[0][0]);
        brushProgram->setSampler("u_texture", brushTex);
        brushProgram->bind();
        renderQuad();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, m_width / 3, m_height / 3);
	glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    textureProgram->setSampler("u_texture", canvasTex);
    textureProgram->bind();
    renderQuad();
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

static glm::vec3 getNDCCoord(const float& x, const float& y, const int& width, const int& height)
{
    glm::vec3 res = glm::vec3((x) / width, (height - y) / height, 0.0f);
    res.x = res.x * 2.0f - 1.0f;
    res.y = res.y * 2.0f - 1.0f;
    //res.z = res.z * 2.0f - 1.0f;
    return res;
}

EFFECTS_NAMESPACE_END