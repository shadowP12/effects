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
EFFECTS_NAMESPACE_BEGIN
    static glm::mat4 gViewMatrix;
    static glm::mat4 gProjMatrix;
    static std::string gTextureVertSource;
    static std::string gTextureFragSource;
    static std::string gDebugVertSource;
    static std::string gDebugFragSource;
    static GfxTexture* gDrawingTexture = nullptr;
    static GfxSampler* gDrawingSampler = nullptr;
    static GfxProgram* gDebugProgram = nullptr;
    static GfxProgram* gTextureProgram = nullptr;
    static Mesh* gQuadMesh = nullptr;
    static DebugPoints* gDebugPoints = nullptr;
    static DebugLines* gDebugLines = nullptr;

    DebugEffect::DebugEffect(int width, int height)
        :BaseEffect(width, height)
    {
    }

    DebugEffect::~DebugEffect()
    {
        if(gQuadMesh)
            delete gQuadMesh;
        destroyGfxTexture(gDrawingTexture);
        destroyGfxSampler(gDrawingSampler);
        destroyGfxProgram(gDebugProgram);
        destroyGfxProgram(gTextureProgram);
        delete gDebugPoints;
        delete gDebugLines;
    }

    void DebugEffect::prepare()
    {
        readFileData("./BuiltinResources/Shaders/common/debug.vs", gDebugVertSource);
        readFileData("./BuiltinResources/Shaders/common/debug.fs", gDebugFragSource);
        readFileData("./BuiltinResources/Shaders/common/texture.vs", gTextureVertSource);
        readFileData("./BuiltinResources/Shaders/common/texture.fs", gTextureFragSource);

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

        gQuadMesh = genQuadMesh();

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
    }

    void DebugEffect::update(float t)
    {
        BaseEffect::update(t);
        if(m_context->getInput()->m_mouse_button_down[0])
        {
            printf("%f   %f\n", m_context->getInput()->m_mouse_position.x, m_context->getInput()->m_mouse_position.y);
            glm::vec3 worldPoint = unProject(glm::vec4(0.0f, 0.0f, m_width, m_height), m_context->getInput()->m_mouse_position, 0.1f, gProjMatrix * gViewMatrix);
            gDebugPoints->addPoint(&worldPoint[0], &glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)[0]);

            glm::mat4 invViewMatrix = glm::inverse(gViewMatrix);
            glm::vec3 cameraPos = getTranslate(invViewMatrix);
            glm::vec3 centerPos = glm::vec3(0.0f);
            glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f);
            glm::vec3 normCenter = glm::normalize(centerPos - cameraPos);
            glm::vec3 plane = glm::cross(normCenter, axis);
            glm::vec3 vec = projectPointToVector(worldPoint, plane);
            vec = worldPoint - vec;
            glm::vec3 v = centerPos + vec;
            glm::vec3 norm = glm::normalize(v - cameraPos);

            gDebugPoints->addPoint(&v[0], &glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)[0]);

            glm::vec3 target = v + norm * 1000.0f;
            gDebugLines->addLine(&target[0],
                                 &v[0],
                                 &glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)[0]);
        }

    }

    void DebugEffect::render()
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        gViewMatrix = m_context->getCamera()->getViewMatrix();
        gProjMatrix = m_context->getCamera()->getProjectionMatrix(m_width, m_height);;

        glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        setGfxProgramMat4(gTextureProgram, "u_model", &modelMatrix[0][0]);
        setGfxProgramMat4(gTextureProgram, "u_view", &gViewMatrix[0][0]);
        setGfxProgramMat4(gTextureProgram, "u_projection", &gProjMatrix[0][0]);
        setGfxProgramSampler(gTextureProgram, "u_texture", gDrawingTexture);
        bindGfxProgram(gTextureProgram);
        gQuadMesh->draw(GL_TRIANGLES);
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