#pragma once

#include "Core/Base.h"
#include "Core/Gfx/GfxResources.h"
#include <string>
EFFECTS_NAMESPACE_BEGIN

    class Mesh;
    class GfxFramebuffer;

    class GfxRenderbuffer;
    class GfxProgram;
    class GfxTexture;
    class GfxSampler;

    class IBLUtility
    {
    public:
        IBLUtility();
        ~IBLUtility();
        void loadHdrEnvMap(const std::string& path);
        GfxTexture* getHDRMap(){ return mHDRMap; }
        GfxTexture* getEnvMap(){ return mEnvCubeMap; }
        GfxTexture* getIrradianceMap(){ return mIrradianceMap; }
        GfxTexture* getPrefilterMap(){ return mPrefilterMap; }
        GfxTexture* getLUTMap(){ return mLUTMap; }
        GfxFramebuffer* getCaptureFramebuffer(){ return mCaptureFramebuffer; }
    private:
        Mesh* mCubeMesh;
        Mesh* mQuadMesh;
        GfxSampler* mSampler;
        GfxTexture* mHDRMap;
        GfxTexture* mEnvCubeMap;
        GfxTexture* mIrradianceMap;
        GfxTexture* mPrefilterMap;
        GfxTexture* mLUTMap;
        GfxProgram* mEquirectangularToCubemapProgram;
        GfxProgram* mIrradianceMapProgram;
        GfxProgram* mPrefilterMapProgram;
        GfxProgram* mLUTMapProgram;
        GfxFramebuffer* mCaptureFramebuffer;
        GfxRenderbuffer* mCaptureRenderbuffer;
    };

EFFECTS_NAMESPACE_END
