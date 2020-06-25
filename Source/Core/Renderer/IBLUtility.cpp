#include "IBLUtility.h"
#include "../Gfx/GfxResources.h"
#include "../RenderResources/Mesh.h"
#include "../Utility/FileUtility.h"
#include "../../Math/Math.h"
#include "../../Importers/TextureImporter.h"
EFFECTS_NAMESPACE_BEGIN

    static int gStandardWidth = 512;
    static int gStandardHeight = 512;
    static glm::mat4 gCaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    static glm::mat4 gCaptureViews[] = {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    IBLUtility::IBLUtility()
    {
        mCubeMesh = genCubeMesh();

        GfxRenderbufferDesc captureRenderbufferDesc;
        captureRenderbufferDesc.width = gStandardWidth;
        captureRenderbufferDesc.height = gStandardHeight;
        captureRenderbufferDesc.internalformat = GL_DEPTH_COMPONENT24;
        mCaptureRenderbuffer = createGfxRenderbuffer(captureRenderbufferDesc);

        mCaptureFramebuffer = createGfxFramebuffer();
        attachGfxFramebufferRenderBuffer(mCaptureFramebuffer, mCaptureRenderbuffer);

        std::string equirectangularToCubemapVertSource;
        std::string equirectangularToCubemapFragSource;
        readFileData("./BuiltinResources/Shaders/ibl/equirectangularToCubemap.vs", equirectangularToCubemapVertSource);
        readFileData("./BuiltinResources/Shaders/ibl/equirectangularToCubemap.fs", equirectangularToCubemapFragSource);
        GfxProgramDesc equirectangularToCubemapProgramDesc;
        equirectangularToCubemapProgramDesc.vertSource = equirectangularToCubemapVertSource;
        equirectangularToCubemapProgramDesc.fragSource = equirectangularToCubemapFragSource;
        equirectangularToCubemapProgramDesc.define = "";
        mEquirectangularToCubemapProgram = createGfxProgram(equirectangularToCubemapProgramDesc);

        std::string genIrradianceMapVertSource;
        std::string genIrradianceMapFragSource;
        readFileData("./BuiltinResources/Shaders/ibl/genIrradianceMap.vs", genIrradianceMapVertSource);
        readFileData("./BuiltinResources/Shaders/ibl/genIrradianceMap.fs", genIrradianceMapFragSource);
        GfxProgramDesc genIrradianceMapProgramDesc;
        genIrradianceMapProgramDesc.vertSource = genIrradianceMapVertSource;
        genIrradianceMapProgramDesc.fragSource = genIrradianceMapFragSource;
        genIrradianceMapProgramDesc.define = "";
        mGenIrradianceMapProgram = createGfxProgram(genIrradianceMapProgramDesc);

        GfxSamplerDesc samplerDesc;
        mSampler = createGfxSampler(samplerDesc);
    }

    IBLUtility::~IBLUtility()
    {
        if(mCubeMesh)
            delete mCubeMesh;
        destroyGfxTexture(mHDRMap);
        destroyGfxTexture(mEnvCubeMap);
        destroyGfxTexture(mIrradianceMap);
        destroyGfxSampler(mSampler);
        destroyGfxFramebuffer(mCaptureFramebuffer);
        destroyGfxRenderbuffer(mCaptureRenderbuffer);
        destroyGfxProgram(mEquirectangularToCubemapProgram);
        destroyGfxProgram(mGenIrradianceMapProgram);
    }

    void IBLUtility::loadHdrEnvMap(const std::string &path)
    {
        // load hdr texture
        int width, height, channels;
        float* pixels = loadFloatImage(path.c_str(), width, height, channels, true);
        GfxTextureDesc hdrTextureDesc;
        hdrTextureDesc.width = width;
        hdrTextureDesc.height = height;
        hdrTextureDesc.componentType = GL_FLOAT;
        hdrTextureDesc.internalFormat = GL_RGB16F;
        hdrTextureDesc.format = GL_RGB;

        mHDRMap = createGfxTexture(hdrTextureDesc);
        writeGfxTextureData(mHDRMap, pixels);
        setGfxTextureSampler(mHDRMap, mSampler);
        delete[] pixels;

        // create env env map
        GfxTextureDesc envTextureDesc;
        envTextureDesc.width = gStandardWidth;
        envTextureDesc.height = gStandardHeight;
        envTextureDesc.componentType = GL_FLOAT;
        envTextureDesc.format = GL_RGB;
        envTextureDesc.internalFormat = GL_RGB16F;
        envTextureDesc.arraySize = 6;
        envTextureDesc.depth = 1;
        mEnvCubeMap = createGfxTexture(envTextureDesc);
        writeGfxTextureData(mEnvCubeMap, nullptr, 0);
        writeGfxTextureData(mEnvCubeMap, nullptr, 1);
        writeGfxTextureData(mEnvCubeMap, nullptr, 2);
        writeGfxTextureData(mEnvCubeMap, nullptr, 3);
        writeGfxTextureData(mEnvCubeMap, nullptr, 4);
        writeGfxTextureData(mEnvCubeMap, nullptr, 5);
        setGfxTextureSampler(mEnvCubeMap, mSampler);

        glViewport(0, 0, gStandardWidth, gStandardHeight);
        for (int i = 0; i < 6; ++i)
        {
            attachGfxFramebufferCubeMap(mCaptureFramebuffer, 0, i, mEnvCubeMap);
            bindGfxFramebuffer(mCaptureFramebuffer);
            glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            setGfxProgramMat4(mEquirectangularToCubemapProgram, "u_projection", &gCaptureProjection[0][0]);
            setGfxProgramMat4(mEquirectangularToCubemapProgram, "u_view", &gCaptureViews[i][0][0]);
            setGfxProgramSampler(mEquirectangularToCubemapProgram, "u_equirectangularMap", mHDRMap);
            bindGfxProgram(mEquirectangularToCubemapProgram);
            mCubeMesh->draw(GL_TRIANGLES);
            unbindGfxProgram(mEquirectangularToCubemapProgram);
            unbindGfxFramebuffer(mCaptureFramebuffer);
        }

        // create irradiance map
        GfxTextureDesc irradianceTextureDesc;
        irradianceTextureDesc.width = 32;
        irradianceTextureDesc.height = 32;
        irradianceTextureDesc.componentType = GL_FLOAT;
        irradianceTextureDesc.format = GL_RGB;
        irradianceTextureDesc.internalFormat = GL_RGB16F;
        irradianceTextureDesc.arraySize = 6;
        irradianceTextureDesc.depth = 1;
        mIrradianceMap = createGfxTexture(irradianceTextureDesc);
        writeGfxTextureData(mIrradianceMap, nullptr, 0);
        writeGfxTextureData(mIrradianceMap, nullptr, 1);
        writeGfxTextureData(mIrradianceMap, nullptr, 2);
        writeGfxTextureData(mIrradianceMap, nullptr, 3);
        writeGfxTextureData(mIrradianceMap, nullptr, 4);
        writeGfxTextureData(mIrradianceMap, nullptr, 5);
        setGfxTextureSampler(mIrradianceMap, mSampler);

        glViewport(0, 0, 32, 32);
        for (int i = 0; i < 6; ++i)
        {
            attachGfxFramebufferCubeMap(mCaptureFramebuffer, 0, i, mIrradianceMap);
            bindGfxFramebuffer(mCaptureFramebuffer);
            glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            setGfxProgramMat4(mGenIrradianceMapProgram, "u_projection", &gCaptureProjection[0][0]);
            setGfxProgramMat4(mGenIrradianceMapProgram, "u_view", &gCaptureViews[i][0][0]);
            setGfxProgramCubeMapSampler(mGenIrradianceMapProgram, "u_environmentMap", mEnvCubeMap);
            bindGfxProgram(mGenIrradianceMapProgram);
            mCubeMesh->draw(GL_TRIANGLES);
            unbindGfxProgram(mGenIrradianceMapProgram);
            unbindGfxFramebuffer(mCaptureFramebuffer);
        }

        // 
    }
EFFECTS_NAMESPACE_END
