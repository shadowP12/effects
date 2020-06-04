#include "PBREffect.h"
#include "../Core/Utility/FileUtility.h"
#include "../UI/LightWidget.h"
#include "../Core/Gfx/GfxDebug.h"
#include "../Core/Gfx/GfxResources.h"
#include "../Core/Renderer/RenderHelper.h"
#include "../Importers/GltfImporter.h"
#include "../Core/Scene/Node.h"
#include "../Core/RenderResources/Mesh.h"
#include "../Core/Datas/MeshData.h"
#include "../Core/Utility/Hash.h"
#include "../Core/Utility/FileUtility.h"
EFFECTS_NAMESPACE_BEGIN

static GltfScene* gScene = nullptr;
static GltfImporter gImporter;
static std::string gVertSource;
static std::string gFragSource;
static std::map<size_t, GfxProgram*> gProgramCache;
static GfxProgram* getPBRProgram(uint32_t material, uint32_t layout);
static std::string getPBRDefine(uint32_t material, uint32_t layout);

PBREffect::PBREffect(int width, int height)
	:BaseEffect(width, height)
{
}

PBREffect::~PBREffect()
{
    if(gScene)
        delete gScene;
    if(gScene)
        delete gScene;
    for (auto& var : gProgramCache)
    {
        destroyGfxProgram(var.second);
    }
}

void PBREffect::prepare()
{
    readFileData("./BuiltinResources/Shaders/pbr.vs", gVertSource);
    readFileData("./BuiltinResources/Shaders/pbr.fs", gFragSource);

    gScene = new GltfScene();
    gImporter.load("./BuiltinResources/Scenes/tf/scene.gltf", gScene);
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
	    GltfMaterial* material = gScene->materialHelper[mesh];
	    GfxProgram* program = getPBRProgram(material->bits, mesh->getLayout());
        glm::mat4 model = node->getWorldMatrix();
        glm::mat4 view = m_context->getCamera()->getViewMatrix();
        glm::mat4 proj = m_context->getCamera()->getProjectionMatrix(m_width, m_height);
        setGfxProgramMat4(program, "u_model", &model[0][0]);
        setGfxProgramMat4(program, "u_view", &view[0][0]);
        setGfxProgramMat4(program, "u_projection", &proj[0][0]);
        setGfxProgramFloat(program, "u_lightRadius", m_light->radius);
        setGfxProgramFloat(program, "u_lightIntensity", m_light->intensity);
        setGfxProgramFloat3(program, "u_lightPos", &m_light->position[0]);
        setGfxProgramFloat3(program, "u_lightColor", &lightColor[0]);
        setGfxProgramFloat3(program, "u_albedo", &albedo[0]);
        bindGfxProgram(program);
        mesh->draw(GL_TRIANGLES);
        unbindGfxProgram(program);
	}
    glEnable(GL_DEPTH_TEST);
}

static std::string getPBRDefine(uint32_t material, uint32_t layout)
{
    std::string define;
    if((material & SEMANTIC_POSITION) != 0)
    {
        define += "#define USE_BASE_COLOR_MAP \n";
    }
    return define;
}

    static GfxProgram* getPBRProgram(uint32_t material, uint32_t layout)
    {
        size_t hash = 0;
        Hash(hash, material);
        Hash(hash, layout);

        auto it = gProgramCache.find(hash);
        if (it == gProgramCache.end())
        {
            GfxProgramDesc programDesc;
            programDesc.vertSource = gVertSource;
            programDesc.fragSource = gFragSource;
            programDesc.define = getPBRDefine(material, layout);
            gProgramCache[hash] = createGfxProgram(programDesc);
            return gProgramCache[hash];
        }
        return gProgramCache[hash];
    }

EFFECTS_NAMESPACE_END