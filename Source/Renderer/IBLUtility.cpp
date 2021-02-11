#include "IBLUtility.h"
#include "Core/Gfx/GfxResources.h"
#include "Resources/Mesh.h"
#include "Core/Utility/FileUtility.h"
#include "Math/Math.h"
#include "Importers/TextureImporter.h"
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

        mQuadMesh = genQuadMesh();

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

        std::string irradianceMapVertSource;
        std::string irradianceMapFragSource;
        readFileData("./BuiltinResources/Shaders/ibl/irradianceMap.vs", irradianceMapVertSource);
        readFileData("./BuiltinResources/Shaders/ibl/irradianceMap.fs", irradianceMapFragSource);
        GfxProgramDesc genIrradianceMapProgramDesc;
        genIrradianceMapProgramDesc.vertSource = irradianceMapVertSource;
        genIrradianceMapProgramDesc.fragSource = irradianceMapFragSource;
        genIrradianceMapProgramDesc.define = "";
        mIrradianceMapProgram = createGfxProgram(genIrradianceMapProgramDesc);

        std::string prefilterMapVertSource;
        std::string prefilterMapFragSource;
        readFileData("./BuiltinResources/Shaders/ibl/prefilterMap.vs", prefilterMapVertSource);
        readFileData("./BuiltinResources/Shaders/ibl/prefilterMap.fs", prefilterMapFragSource);
        GfxProgramDesc prefilterMapProgramDesc;
        prefilterMapProgramDesc.vertSource = prefilterMapVertSource;
        prefilterMapProgramDesc.fragSource = prefilterMapFragSource;
        prefilterMapProgramDesc.define = "";
        mPrefilterMapProgram = createGfxProgram(prefilterMapProgramDesc);

        std::string lutMapVertSource;
        std::string lutMapFragSource;
        readFileData("./BuiltinResources/Shaders/ibl/brdfLUT.vs", lutMapVertSource);
        readFileData("./BuiltinResources/Shaders/ibl/brdfLUT.fs", lutMapFragSource);
        GfxProgramDesc lutMapProgramDesc;
        lutMapProgramDesc.vertSource = lutMapVertSource;
        lutMapProgramDesc.fragSource = lutMapFragSource;
        lutMapProgramDesc.define = "";
        mLUTMapProgram = createGfxProgram(lutMapProgramDesc);

        GfxSamplerDesc samplerDesc;
        mSampler = createGfxSampler(samplerDesc);
    }

    IBLUtility::~IBLUtility()
    {
        destroyGfxTexture(mHDRMap);
        destroyGfxTexture(mEnvCubeMap);
        destroyGfxTexture(mIrradianceMap);
        destroyGfxTexture(mPrefilterMap);
        destroyGfxTexture(mLUTMap);
        destroyGfxSampler(mSampler);
        destroyGfxFramebuffer(mCaptureFramebuffer);
        destroyGfxRenderbuffer(mCaptureRenderbuffer);
        destroyGfxProgram(mEquirectangularToCubemapProgram);
        destroyGfxProgram(mIrradianceMapProgram);
        destroyGfxProgram(mPrefilterMapProgram);
        destroyGfxProgram(mLUTMapProgram);
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
        envTextureDesc.mipmap = true;
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
        genGfxTextureMipmap(mEnvCubeMap);

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
            setGfxProgramMat4(mIrradianceMapProgram, "u_projection", &gCaptureProjection[0][0]);
            setGfxProgramMat4(mIrradianceMapProgram, "u_view", &gCaptureViews[i][0][0]);
            setGfxProgramCubeMapSampler(mIrradianceMapProgram, "u_environmentMap", mEnvCubeMap);
            bindGfxProgram(mIrradianceMapProgram);
            mCubeMesh->draw(GL_TRIANGLES);
            unbindGfxProgram(mIrradianceMapProgram);
            unbindGfxFramebuffer(mCaptureFramebuffer);
        }

        // create prefilter map
        GfxTextureDesc prefilterTextureDesc;
        prefilterTextureDesc.width = 128;
        prefilterTextureDesc.height = 128;
        prefilterTextureDesc.componentType = GL_FLOAT;
        prefilterTextureDesc.format = GL_RGB;
        prefilterTextureDesc.internalFormat = GL_RGB16F;
        prefilterTextureDesc.arraySize = 6;
        prefilterTextureDesc.depth = 1;
        prefilterTextureDesc.mipmap = true;
        mPrefilterMap = createGfxTexture(prefilterTextureDesc);
        writeGfxTextureData(mPrefilterMap, nullptr, 0);
        writeGfxTextureData(mPrefilterMap, nullptr, 1);
        writeGfxTextureData(mPrefilterMap, nullptr, 2);
        writeGfxTextureData(mPrefilterMap, nullptr, 3);
        writeGfxTextureData(mPrefilterMap, nullptr, 4);
        writeGfxTextureData(mPrefilterMap, nullptr, 5);
        mSampler->minFilter = GL_LINEAR_MIPMAP_LINEAR;
        setGfxTextureSampler(mPrefilterMap, mSampler);
        genGfxTextureMipmap(mPrefilterMap);

        uint32_t maxMipLevels = 5;
        for (int mip = 0; mip < maxMipLevels; ++mip)
        {
            uint32_t mipWidth = 128 * std::pow(0.5, mip);
            uint32_t mipHeight = 128 * std::pow(0.5, mip);
            float roughness = (float)mip / (float)(maxMipLevels - 1);
            glViewport(0, 0, mipWidth, mipHeight);
            for (int i = 0; i < 6; ++i)
            {
                attachGfxFramebufferCubeMap(mCaptureFramebuffer, 0, i, mPrefilterMap, mip);
                bindGfxFramebuffer(mCaptureFramebuffer);
                glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                setGfxProgramMat4(mPrefilterMapProgram, "u_projection", &gCaptureProjection[0][0]);
                setGfxProgramMat4(mPrefilterMapProgram, "u_view", &gCaptureViews[i][0][0]);
                setGfxProgramCubeMapSampler(mPrefilterMapProgram, "u_environmentMap", mEnvCubeMap);
                setGfxProgramFloat(mPrefilterMapProgram, "u_roughness", roughness);
                setGfxProgramFloat(mPrefilterMapProgram, "u_resolution", 512.0f);
                bindGfxProgram(mPrefilterMapProgram);
                mCubeMesh->draw(GL_TRIANGLES);
                unbindGfxProgram(mPrefilterMapProgram);
                unbindGfxFramebuffer(mCaptureFramebuffer);
            }
        }

        // create lut map
        GfxTextureDesc lutTextureDesc;
        lutTextureDesc.width = 512;
        lutTextureDesc.height = 512;
        lutTextureDesc.componentType = GL_FLOAT;
        lutTextureDesc.format = GL_RGB;
        lutTextureDesc.internalFormat = GL_RGB16F;
        mLUTMap = createGfxTexture(lutTextureDesc);
        writeGfxTextureData(mLUTMap, nullptr);
        mSampler->minFilter = GL_LINEAR;
        setGfxTextureSampler(mLUTMap, mSampler);

        glViewport(0, 0, 512, 512);
        attachGfxFramebufferTexture(mCaptureFramebuffer, 0, mLUTMap);
        bindGfxFramebuffer(mCaptureFramebuffer);
        glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        bindGfxProgram(mLUTMapProgram);
        mQuadMesh->draw(GL_TRIANGLES);
        unbindGfxProgram(mLUTMapProgram);
        unbindGfxFramebuffer(mCaptureFramebuffer);
    }
EFFECTS_NAMESPACE_END
