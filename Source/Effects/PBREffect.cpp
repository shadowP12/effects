#include "PBREffect.h"
#include "../Core/Utility/FileUtility.h"
#include "../UI/LightWidget.h"
#include "../Core/Gfx/GfxDebug.h"
#include "../Core/Gfx/GfxResources.h"
#include "../Core/Renderer/RenderHelper.h"
#include "../Importers/GltfImporter.h"
#include "../Core/Scene/Node.h"
#include "../Core/RenderResources/Mesh.h"
EFFECTS_NAMESPACE_BEGIN

static GltfScene* gScene = nullptr;
static GltfImporter gImporter;
static GfxProgram* gPBRProgram = nullptr;

PBREffect::PBREffect(int width, int height)
	:BaseEffect(width, height)
{
}

PBREffect::~PBREffect()
{
    if(gScene)
        delete gScene;
    if(gPBRProgram)
        delete gPBRProgram;
    if(gScene)
        delete gScene;
}

void PBREffect::prepare()
{
    gPBRProgram = loadProgram(getCurrentPath() + R"(\BuiltinResources\Shaders\pbr.vs)",
                                 getCurrentPath() + R"(\BuiltinResources\Shaders\pbr.fs)");
    gScene = new GltfScene();
    gImporter.load(getCurrentPath() + R"(\BuiltinResources\Scenes\tf\scene.gltf)", gScene);

	m_light = new Light();
	m_light_widget = new LightWidget(m_light);
	m_context->getUISystem()->addWidget(m_light_widget);
}

void PBREffect::update(float t)
{
    BaseEffect::update(t);
}

void PBREffect::render()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 albedo = glm::vec3(0.8f, 0.3f, 0.3f);
    glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);

	for (auto& info : gScene->meshHelper)
	{
	    std::shared_ptr<Node> node = info.first;
	    Mesh* mesh = info.second;
        glm::mat4 model = node->getWorldMatrix();
        glm::mat4 view = m_context->getCamera()->getViewMatrix();
        glm::mat4 proj = m_context->getCamera()->getProjectionMatrix(m_width, m_height);
        gPBRProgram->setMat4("u_model",  &model[0][0]);
        gPBRProgram->setMat4("u_view",  &view[0][0]);
        gPBRProgram->setMat4("u_projection",  &proj[0][0]);
        gPBRProgram->setFloat("u_lightRadius", m_light->radius);
        gPBRProgram->setFloat("u_lightIntensity", m_light->intensity);
        gPBRProgram->setFloat3("u_lightPos", &m_light->position[0]);
        gPBRProgram->setFloat3("u_lightColor", &lightColor[0]);
        gPBRProgram->setFloat3("u_albedo", &albedo[0]);
        gPBRProgram->bind();
        mesh->draw(GL_TRIANGLES);
	}
    glEnable(GL_DEPTH_TEST);
}

EFFECTS_NAMESPACE_END