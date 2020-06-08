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
#include "../Core/RenderResources/Mesh.h"
#include "../Core/Utility/Hash.h"
#include "../Core/Utility/FileUtility.h"
EFFECTS_NAMESPACE_BEGIN

    static GltfScene* gScene = nullptr;
    static GltfImporter gImporter;
    static glm::mat4 gViewMatrix;
    static glm::mat4 gProjMatrix;
    static std::string gPBRVertSource;
    static std::string gPBRFragSource;
    static std::string gOITVertSource;
    static std::string gOITFragSource;
    static GfxTexture* gDrawingTexture = nullptr;
    static GfxSampler* gDrawingSampler = nullptr;
    static GfxFramebuffer * gDrawingFramebuffer = nullptr;
    static GfxRenderbuffer* gDrawingDepthBuffer;
    static GfxProgram* gOITProgram = nullptr;
    static GfxTexture* gOITTextureA = nullptr;
    static GfxTexture* gOITTextureB = nullptr;
    static GfxSampler* gOITSampler = nullptr;
    static GfxFramebuffer * gOITFramebuffer = nullptr;
    static std::map<size_t, GfxProgram*> gProgramCache;
    static Mesh* gQuadMesh = nullptr;
    static GfxProgram* getPBRProgram(uint32_t material, uint32_t layout);
    static std::string getPBRDefine(uint32_t material, uint32_t layout);

typedef struct RenderElement
{
    std::shared_ptr<Node> node;
    Mesh* mesh;
    GltfMaterial* material;
} RenderElement;

static std::vector<RenderElement> gOpaqueQueue;
static std::vector<RenderElement> gTransparentQueue;

static void drawOpaqueSurfaces();
static void drawTransparentSurfaces();

PBREffect::PBREffect(int width, int height)
	:BaseEffect(width, height)
{
}

PBREffect::~PBREffect()
{
    destroyGltfScene(gScene);

    for (auto& var : gProgramCache)
    {
        destroyGfxProgram(var.second);
    }

    if(gQuadMesh)
        delete gQuadMesh;
    destroyGfxTexture(gDrawingTexture);
    destroyGfxSampler(gDrawingSampler);
    destroyGfxFramebuffer(gDrawingFramebuffer);
    destroyGfxRenderbuffer(gDrawingDepthBuffer);

    destroyGfxTexture(gOITTextureA);
    destroyGfxTexture(gOITTextureB);
    destroyGfxSampler(gOITSampler);
    destroyGfxFramebuffer(gOITFramebuffer);
    destroyGfxProgram(gOITProgram);
}

void PBREffect::prepare()
{
    readFileData("./BuiltinResources/Shaders/pbr.vs", gPBRVertSource);
    readFileData("./BuiltinResources/Shaders/pbr.fs", gPBRFragSource);
    readFileData("./BuiltinResources/Shaders/oit.vs", gOITVertSource);
    readFileData("./BuiltinResources/Shaders/oit.fs", gOITFragSource);

    GfxSamplerDesc samplerDesc;
    samplerDesc.minFilter = GL_NEAREST;
    samplerDesc.magFilter = GL_NEAREST;
    samplerDesc.wrapS = GL_REPEAT;
    samplerDesc.wrapT = GL_REPEAT;
    gDrawingSampler = createGfxSampler(samplerDesc);
    gOITSampler = createGfxSampler(samplerDesc);

    GfxTextureDesc drawingTexDesc;
    drawingTexDesc.width = m_width;
    drawingTexDesc.height = m_height;
    drawingTexDesc.componentType = GL_FLOAT;
    drawingTexDesc.internalFormat = GL_RGBA16F;
    drawingTexDesc.format = GL_RGBA;
    gDrawingTexture = createGfxTexture(drawingTexDesc);
    writeGfxTextureData(gDrawingTexture, nullptr);
    setGfxTextureSampler(gDrawingTexture, gDrawingSampler);

    GfxRenderbufferDesc drawingDepthBufferDesc;
    drawingDepthBufferDesc.width = m_width;
    drawingDepthBufferDesc.height = m_height;
    drawingDepthBufferDesc.internalformat = GL_DEPTH_COMPONENT;
    gDrawingDepthBuffer = createGfxRenderbuffer(drawingDepthBufferDesc);

    GfxFramebufferDesc drawingFramebufferDesc;
    drawingFramebufferDesc.targets[0] = gDrawingTexture;
    drawingFramebufferDesc.depthBuffer = gDrawingDepthBuffer;
    gDrawingFramebuffer = createGfxFramebuffer(drawingFramebufferDesc);

    GfxTextureDesc oitTexDesc;
    oitTexDesc.width = m_width;
    oitTexDesc.height = m_height;
    oitTexDesc.componentType = GL_FLOAT;
    oitTexDesc.internalFormat = GL_RGBA16F;
    oitTexDesc.format = GL_RGBA;
    gOITTextureA = createGfxTexture(oitTexDesc);
    writeGfxTextureData(gOITTextureA, nullptr);
    setGfxTextureSampler(gOITTextureA, gOITSampler);
//    oitTexDesc.internalFormat = GL_RGB16F;
//    oitTexDesc.format = GL_RGB;
    gOITTextureB = createGfxTexture(oitTexDesc);
    writeGfxTextureData(gOITTextureB, nullptr);
    setGfxTextureSampler(gOITTextureB, gOITSampler);

    GfxFramebufferDesc oitFramebufferDesc;
    oitFramebufferDesc.targets[0] = gOITTextureA;
    oitFramebufferDesc.targets[1] = gOITTextureB;
    oitFramebufferDesc.depthBuffer = gDrawingDepthBuffer;
    gOITFramebuffer = createGfxFramebuffer(oitFramebufferDesc);

    GfxProgramDesc oitProgramDesc;
    oitProgramDesc.vertSource = gOITVertSource;
    oitProgramDesc.fragSource = gOITFragSource;
    oitProgramDesc.define = "";
    gOITProgram = createGfxProgram(oitProgramDesc);

    gQuadMesh = genQuadMesh();

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
    gViewMatrix = m_context->getCamera()->getViewMatrix();
    gProjMatrix = m_context->getCamera()->getProjectionMatrix(m_width, m_height);;
    gOpaqueQueue.clear();
    gTransparentQueue.clear();

	for (auto& info : gScene->meshHelper)
	{
	    std::shared_ptr<Node> node = info.first;
	    Mesh* mesh = info.second;
	    GltfMaterial* material = gScene->materialHelper[mesh];
        if(material->alphaMode == ALPHA_MODE_BLEND)
        {
            RenderElement renderElement;
            renderElement.node = node;
            renderElement.mesh = mesh;
            renderElement.material = material;
            gTransparentQueue.push_back(renderElement);
        } else
        {
            RenderElement renderElement;
            renderElement.node = node;
            renderElement.mesh = mesh;
            renderElement.material = material;
            gOpaqueQueue.push_back(renderElement);
        }
	}


    glEnable(GL_DEPTH_TEST);
	drawOpaqueSurfaces();
    glDisable(GL_DEPTH_TEST);

	bindGfxFramebuffer(gOITFramebuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    drawOpaqueSurfaces();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
	drawTransparentSurfaces();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    unbindGfxFramebuffer(gOITFramebuffer);

	// oit post processing
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
    setGfxProgramSampler(gOITProgram, "u_oitTextureA", gOITTextureA);
    setGfxProgramSampler(gOITProgram, "u_oitTextureB", gOITTextureB);
    bindGfxProgram(gOITProgram);
    gQuadMesh->draw(GL_TRIANGLES);
    unbindGfxProgram(gOITProgram);
    glDisable(GL_BLEND);
}

static std::string getPBRDefine(uint32_t material, uint32_t layout)
{
    std::string define;
    if((material & PBR_ALPHA) != 0)
    {
        define += "#define USE_ALPHA_MODE \n";
    }
    if((material & PBR_BASE_COLOR_MAP) != 0)
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
            programDesc.vertSource = gPBRVertSource;
            programDesc.fragSource = gPBRFragSource;
            programDesc.define = getPBRDefine(material, layout);
            gProgramCache[hash] = createGfxProgram(programDesc);
            return gProgramCache[hash];
        }
        return gProgramCache[hash];
    }

    static void drawOpaqueSurfaces()
    {
        for (int i = 0; i < gOpaqueQueue.size(); ++i)
        {
            std::shared_ptr<Node> node = gOpaqueQueue[i].node;
            Mesh* mesh = gOpaqueQueue[i].mesh;
            GltfMaterial* material = gOpaqueQueue[i].material;
            GfxProgram* program = getPBRProgram(material->bits, mesh->getLayout());

            // seting pipeline state
            if(!material->doubleSided)
            {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
            }

            glm::mat4 model = node->getWorldMatrix();
            setGfxProgramMat4(program, "u_model", &model[0][0]);
            setGfxProgramMat4(program, "u_view", &gViewMatrix[0][0]);
            setGfxProgramMat4(program, "u_projection", &gProjMatrix[0][0]);
            // seting base color
            setGfxProgramFloat4(program, "u_baseColor", &material->baseColor[0]);
            if((material->bits & PBR_BASE_COLOR_MAP) != 0)
            {
                setGfxTextureSampler(material->baseColorMap.texture, material->baseColorMap.sampler);
                setGfxProgramSampler(program, "u_baseColorMap", material->baseColorMap.texture);
            }

            bindGfxProgram(program);
            mesh->draw(GL_TRIANGLES);
            unbindGfxProgram(program);

            // reset pipeline state
            if(!material->doubleSided)
            {
                glDisable(GL_CULL_FACE);
            }
        }
    }

    static void drawTransparentSurfaces()
    {
        for (int i = 0; i < gTransparentQueue.size(); ++i)
        {
            std::shared_ptr<Node> node = gTransparentQueue[i].node;
            Mesh* mesh = gTransparentQueue[i].mesh;
            GltfMaterial* material = gTransparentQueue[i].material;
            GfxProgram* program = getPBRProgram(material->bits, mesh->getLayout());

            // seting pipeline state
            if(!material->doubleSided)
            {
//                glEnable(GL_CULL_FACE);
//                glCullFace(GL_BACK);
            }
//            if(material->alphaMode == ALPHA_MODE_BLEND)
//            {
//                glEnable(GL_BLEND);
//                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//            }

            glm::mat4 model = node->getWorldMatrix();
            setGfxProgramMat4(program, "u_model", &model[0][0]);
            setGfxProgramMat4(program, "u_view", &gViewMatrix[0][0]);
            setGfxProgramMat4(program, "u_projection", &gProjMatrix[0][0]);
            // seting base color
            setGfxProgramFloat4(program, "u_baseColor", &material->baseColor[0]);
            if((material->bits & PBR_BASE_COLOR_MAP) != 0)
            {
                setGfxTextureSampler(material->baseColorMap.texture, material->baseColorMap.sampler);
                setGfxProgramSampler(program, "u_baseColorMap", material->baseColorMap.texture);
            }

            bindGfxProgram(program);
            mesh->draw(GL_TRIANGLES);
            unbindGfxProgram(program);

            // reset pipeline state
            if(!material->doubleSided)
            {
                //glDisable(GL_CULL_FACE);
            }
//            if(material->alphaMode == ALPHA_MODE_BLEND)
//            {
//                glDisable(GL_BLEND);
//            }
        }
    }
EFFECTS_NAMESPACE_END