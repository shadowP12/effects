#include "TerrainEffect.h"
#include "../Core/Utility/FileUtility.h"
#include "../Core/Gfx/GpuProgram.h"
#include "../Core/Gfx/GfxDebug.h"
#include "../Core/Gfx/GfxRenderer.h"
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
static Mesh* gQuadMesh = nullptr;
static GfxRenderer* gRenderer = nullptr;
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
    delete gRenderer;
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

    gQuadMesh = genQuadMesh();

    gRenderer = new GfxRenderer();
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
}

void TerrainEffect::render()
{
    gRenderer->bindFramebuffer(canvasFb);
    gRenderer->setViewport(0, 0, m_width, m_height);
    gRenderer->setClearValue(0.8f, 0.1f, 0.1f, 1.0f);
    gRenderer->setProgram(brushProgram);
    for (int i = 0; i < points.size(); ++i)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0), points[i]);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 1.0f));
        brushProgram->setMat4("u_model", &model[0][0]);
        brushProgram->setSampler("u_texture", brushTex);
        gRenderer->setVertexBuffer(gQuadMesh->getVertexBuffer());
        gRenderer->setVertexLayout(gQuadMesh->getVertexLayout());
        gRenderer->draw(GfxPrimitiveMode::TRIANGLE_STRIP, 0, gQuadMesh->getVertexCount());
    }

    textureProgram->setSampler("u_texture", canvasTex);
    gRenderer->bindFramebuffer();
    gRenderer->setViewport(0, 0, m_width / 3, m_height / 3);
    gRenderer->setClearValue(0.3f, 0.3f, 0.8f, 1.0f);
    gRenderer->setProgram(textureProgram);
    gRenderer->setVertexBuffer(gQuadMesh->getVertexBuffer());
    gRenderer->setVertexLayout(gQuadMesh->getVertexLayout());
    gRenderer->draw(GfxPrimitiveMode::TRIANGLE_STRIP, 0, gQuadMesh->getVertexCount());
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