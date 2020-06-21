#pragma once

#include "../Base.h"
#include "../Gfx/GfxResources.h"
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
        GfxTexture* getHDRMap(){return mHDRMap;}
        GfxTexture* getEnvMap(){return mEnvCubeMap;}
        GfxFramebuffer* getCaptureFramebuffer(){return mCaptureFramebuffer;}
    private:
        Mesh* mCubeMesh;
        GfxSampler* mSampler;
        GfxTexture* mHDRMap;
        GfxTexture* mEnvCubeMap;
        GfxProgram* mEquirectangularToCubemapProgram;
        GfxFramebuffer* mCaptureFramebuffer;
        GfxRenderbuffer* mCaptureRenderbuffer;
    };

EFFECTS_NAMESPACE_END
