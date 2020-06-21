#include "DebugEffect.h"
#include "../Core/Utility/FileUtility.h"
#include "../UI/LightWidget.h"
#include "../Core/Gfx/GfxDebug.h"
#include "../Core/Gfx/GfxResources.h"
#include "../Core/Renderer/RenderHelper.h"
#include "../Importers/GltfImporter.h"
#include "../Importers/TextureImporter.h"
#include "../Core/Scene/Node.h"
#include "../Core/RenderResources/Mesh.h"
#include "../Core/Datas/MeshData.h"
#include "../Core/RenderResources/Mesh.h"
#include "../Core/Utility/Hash.h"
#include "../Core/Utility/FileUtility.h"
#include "../Core/Renderer/IBLUtility.h"
EFFECTS_NAMESPACE_BEGIN
    static glm::mat4 gViewMatrix;
    static glm::mat4 gProjMatrix;
    static std::string gTextureVertSource;
    static std::string gTextureFragSource;
    static std::string gDebugVertSource;
    static std::string gDebugFragSource;
    static std::string gSkyboxVertSource;
    static std::string gSkyboxFragSource;
    static GfxTexture* gDrawingTexture = nullptr;
    static GfxSampler* gDrawingSampler = nullptr;
    static GfxProgram* gDebugProgram = nullptr;
    static GfxProgram* gTextureProgram = nullptr;
    static GfxProgram* gSkyboxProgram = nullptr;
    static Mesh* gQuadMesh = nullptr;
    static Mesh* gCubeMesh = nullptr;
    static DebugPoints* gDebugPoints = nullptr;
    static DebugLines* gDebugLines = nullptr;
    static IBLUtility* gIBL = nullptr;

    DebugEffect::DebugEffect(int width, int height)
        :BaseEffect(width, height)
    {
    }

    DebugEffect::~DebugEffect()
    {
        if(gQuadMesh)
            delete gQuadMesh;
        if(gCubeMesh)
            delete gCubeMesh;
        if(gIBL)
            delete gIBL;
        destroyGfxTexture(gDrawingTexture);
        destroyGfxSampler(gDrawingSampler);
        destroyGfxProgram(gDebugProgram);
        destroyGfxProgram(gTextureProgram);
        destroyGfxProgram(gSkyboxProgram);
        delete gDebugPoints;
        delete gDebugLines;
    }

    void DebugEffect::prepare()
    {
        readFileData("./BuiltinResources/Shaders/common/debug.vs", gDebugVertSource);
        readFileData("./BuiltinResources/Shaders/common/debug.fs", gDebugFragSource);
        readFileData("./BuiltinResources/Shaders/common/texture.vs", gTextureVertSource);
        readFileData("./BuiltinResources/Shaders/common/texture.fs", gTextureFragSource);
        readFileData("./BuiltinResources/Shaders/common/skybox.vs", gSkyboxVertSource);
        readFileData("./BuiltinResources/Shaders/common/skybox.fs", gSkyboxFragSource);
        GfxSamplerDesc samplerDesc;
        samplerDesc.minFilter = GL_NEAREST;
        samplerDesc.magFilter = GL_NEAREST;
        samplerDesc.wrapS = GL_REPEAT;
        samplerDesc.wrapT = GL_REPEAT;
        gDrawingSampler = createGfxSampler(samplerDesc);

        int width, height, channels;
        unsigned char* pixels = loadImage("./BuiltinResources/Textures/test.png", width, height, channels);
        GfxTextureDesc textureDesc;
        textureDesc.width = width;
        textureDesc.height = height;
        textureDesc.componentType = GL_UNSIGNED_BYTE;
        if (channels == 4)
        {
            textureDesc.internalFormat = GL_RGBA8;
            textureDesc.format = GL_RGBA;
        }
        else if (channels == 3)
        {
            textureDesc.internalFormat = GL_RGB8;
            textureDesc.format = GL_RGB;
        }
        gDrawingTexture = createGfxTexture(textureDesc);
        writeGfxTextureData(gDrawingTexture, pixels);
        setGfxTextureSampler(gDrawingTexture, gDrawingSampler);
        delete[] pixels;

        GfxProgramDesc debugProgramDesc;
        debugProgramDesc.vertSource = gDebugVertSource;
        debugProgramDesc.fragSource = gDebugFragSource;
        debugProgramDesc.define = "";
        gDebugProgram = createGfxProgram(debugProgramDesc);

        GfxProgramDesc blitProgramDesc;
        blitProgramDesc.vertSource = gTextureVertSource;
        blitProgramDesc.fragSource = gTextureFragSource;
        blitProgramDesc.define = "";
        gTextureProgram = createGfxProgram(blitProgramDesc);

        GfxProgramDesc skyboxProgramDesc;
        skyboxProgramDesc.vertSource = gSkyboxVertSource;
        skyboxProgramDesc.fragSource = gSkyboxFragSource;
        skyboxProgramDesc.define = "";
        gSkyboxProgram = createGfxProgram(skyboxProgramDesc);

        gQuadMesh = genQuadMesh();
        gCubeMesh = genCubeMesh();

        gDebugPoints = new DebugPoints;
        gDebugLines = new DebugLines;

        gDebugLines->addLine(&glm::vec3(0.0f, 0.0f, 0.0f)[0],
                &glm::vec3(1.0f, 0.0f, 0.0f)[0],
                &glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)[0]);

        gDebugLines->addLine(&glm::vec3(0.0f, 0.0f, 0.0f)[0],
                             &glm::vec3(0.0f, 1.0f, 0.0f)[0],
                             &glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)[0]);

        gDebugLines->addLine(&glm::vec3(0.0f, 0.0f, -10000.0f)[0],
                             &glm::vec3(0.0f, 0.0f, 10000.0f)[0],
                             &glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)[0]);

        gIBL = new IBLUtility();
        gIBL->loadHdrEnvMap("./BuiltinResources/Textures/newport_loft.hdr");
    }

    void DebugEffect::update(float t)
    {
        BaseEffect::update(t);
    }

    void DebugEffect::render()
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        gViewMatrix = m_context->getCamera()->getViewMatrix();
        gProjMatrix = m_context->getCamera()->getProjectionMatrix(m_width, m_height);;

        glViewport(0, 0, m_width, m_height);
        glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw skybox
        glm::mat4 viewMatrix = glm::mat4(glm::mat3(gViewMatrix));
        setGfxProgramMat4(gSkyboxProgram, "u_view", &viewMatrix[0][0]);
        setGfxProgramMat4(gSkyboxProgram, "u_projection", &gProjMatrix[0][0]);
        setGfxProgramCubeMapSampler(gSkyboxProgram, "u_skybox", gIBL->getEnvMap());
        bindGfxProgram(gSkyboxProgram);
        glDepthMask(false);
        gCubeMesh->draw(GL_TRIANGLES);
        glDepthMask(true);
        unbindGfxProgram(gSkyboxProgram);

        setGfxProgramMat4(gTextureProgram, "u_model", &modelMatrix[0][0]);
        setGfxProgramMat4(gTextureProgram, "u_view", &gViewMatrix[0][0]);
        setGfxProgramMat4(gTextureProgram, "u_projection", &gProjMatrix[0][0]);
        setGfxProgramSampler(gTextureProgram, "u_texture", gIBL->getHDRMap());
        bindGfxProgram(gTextureProgram);
        glEnable(GL_DEPTH_TEST);
        gCubeMesh->draw(GL_TRIANGLES);
        glDisable(GL_DEPTH_TEST);
        unbindGfxProgram(gTextureProgram);

        setGfxProgramMat4(gDebugProgram, "u_model", &glm::mat4(1.0f)[0][0]);
        setGfxProgramMat4(gDebugProgram, "u_view", &gViewMatrix[0][0]);
        setGfxProgramMat4(gDebugProgram, "u_projection", &gProjMatrix[0][0]);
        bindGfxProgram(gDebugProgram);
        glPointSize(10.0f);
        gDebugLines->draw();
        gDebugPoints->draw();
        glPointSize(1.0f);
        bindGfxProgram(gDebugProgram);
    }
EFFECTS_NAMESPACE_END