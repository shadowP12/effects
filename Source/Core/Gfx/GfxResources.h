#pragma once
#include "../Base.h"
#include "Gfx.h"
#include "GfxCommonTypes.h"
#include <string>
EFFECTS_NAMESPACE_BEGIN

class GfxBuffer;
class GfxTexture;
class GfxFramebuffer;
class GfxProgram;

struct GfxBufferDesc
{
    int size;
    GfxBufferUsageBit bufferUsage;
    GfxMemoryUsageBit memUsage;
    GfxBufferAccessBit bufferAccess;
};

class GfxBuffer
{
    friend class GfxRenderObj;
public:
    GfxBuffer(const GfxBufferDesc& desc);
    ~GfxBuffer();
    void writeData(const void* data, const int& size);
    void writeData(const void* data, const int& offset, const int& size);
    void resize(int size);
protected:
    GLuint mHandle;
    int mSize;
    GfxBufferUsageBit mBufferUsage;
    GfxMemoryUsageBit mMemUsage;
    GfxBufferAccessBit mBufferAccess;
};

typedef struct GfxSamplerDesc
{
    uint32_t magFilter;
    uint32_t minFilter;
    uint32_t wrapS;
    uint32_t wrapT;
} GfxSamplerDesc;

typedef struct GfxSampler
{
    uint32_t magFilter;
    uint32_t minFilter;
    uint32_t wrapS;
    uint32_t wrapT;
} GfxSampler;

    GfxSampler* createGfxSampler(const GfxSamplerDesc& desc);
    void destroyGfxSampler(GfxSampler* sampler);

typedef struct GfxTextureDesc
{
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    uint32_t arraySize;
    uint32_t format;
    uint32_t internalFormat;
    uint32_t componentType;
} GfxTextureDesc;

typedef struct GfxTexture
{
    uint32_t handle;
    uint32_t width;
    uint32_t height;
    uint32_t depth = 1;
    uint32_t arraySize = 1;
    uint32_t format;
    uint32_t internalFormat;
    uint32_t componentType;
} GfxTexture;

    GfxTexture* createGfxTexture(const GfxTextureDesc& desc);
    void destroyGfxTexture(GfxTexture* tex);
    void writeGfxTextureData(const GfxTexture* tex, void* data, uint32_t arraySize = 1, uint32_t depth = 1);
    void setGfxTextureSampler(const GfxTexture* tex, const GfxSampler* sampler);

typedef struct GfxFramebufferDesc
{
    GfxTexture* targets[8] = {nullptr};
} GfxFramebufferDesc;

class GfxFramebuffer
{
    friend class GfxRenderObj;
public:
    GfxFramebuffer(const GfxFramebufferDesc& desc);
    ~GfxFramebuffer();
    void bind();

protected:
    GLuint mHandle;
    GfxTexture* mTatgets[8];
};

class GfxProgram
{
    friend class GfxRenderObj;
public:
    GfxProgram(const char* vs, const char* fs);
    ~GfxProgram();
    void bind();
    struct FloatParam
    {
        std::string name;
        float value;
    };
    struct Float2Param
    {
        std::string name;
        float value[2];
    };
    struct Float3Param
    {
        std::string name;
        float value[3];
    };
    struct Float4Param
    {
        std::string name;
        float value[4];
    };
    struct Mat4Param
    {
        std::string name;
        float value[16];
    };
    struct Sampler2DParam
    {
        std::string name;
        GfxTexture* texture;
    };
    void setFloat(const char* name, float value)
    {
        for (int i = 0; i < mFloatParamCount; ++i)
        {
            if(mFloatParams[i].name == name)
            {
                mFloatParams[i].value = value;
                return;
            }
        }
        if(mFloatParamCount >= 8)
            return;
        mFloatParams[mFloatParamCount].name = name;
        mFloatParams[mFloatParamCount].value = value;
        mFloatParamCount++;
    }
    void setFloat2(const char* name, const float* value)
    {
        for (int i = 0; i < mFloat2ParamCount; ++i)
        {
            if(mFloat2Params[i].name == name)
            {
                mFloat2Params[i].value[0] = value[0];
                mFloat2Params[i].value[1] = value[1];
                return;
            }
        }
        if(mFloat2ParamCount >= 8)
            return;
        mFloat2Params[mFloat2ParamCount].name = name;
        mFloat2Params[mFloat2ParamCount].value[0] = value[0];
        mFloat2Params[mFloat2ParamCount].value[1] = value[1];
        mFloat2ParamCount++;
    }
    void setFloat2(const char* name, const float& x, const float& y)
    {
        for (int i = 0; i < mFloat2ParamCount; ++i)
        {
            if(mFloat2Params[i].name == name)
            {
                mFloat2Params[i].value[0] = x;
                mFloat2Params[i].value[1] = y;
                return;
            }
        }
        if(mFloat2ParamCount >= 8)
            return;
        mFloat2Params[mFloat2ParamCount].name = name;
        mFloat2Params[mFloat2ParamCount].value[0] = x;
        mFloat2Params[mFloat2ParamCount].value[1] = y;
        mFloat2ParamCount++;
    }
    void setFloat3(const char* name, const float* value)
    {
        for (int i = 0; i < mFloat3ParamCount; ++i)
        {
            if(mFloat3Params[i].name == name)
            {
                mFloat3Params[i].value[0] = value[0];
                mFloat3Params[i].value[1] = value[1];
                mFloat3Params[i].value[2] = value[2];
                return;
            }
        }
        if(mFloat3ParamCount >= 8)
            return;
        mFloat3Params[mFloat3ParamCount].name = name;
        mFloat3Params[mFloat3ParamCount].value[0] = value[0];
        mFloat3Params[mFloat3ParamCount].value[1] = value[1];
        mFloat3Params[mFloat3ParamCount].value[2] = value[2];
        mFloat3ParamCount++;
    }
    void setFloat3(const char* name, const float& x, const float& y, const float& z)
    {
        for (int i = 0; i < mFloat3ParamCount; ++i)
        {
            if(mFloat3Params[i].name == name)
            {
                mFloat3Params[i].value[0] = x;
                mFloat3Params[i].value[1] = y;
                mFloat3Params[i].value[2] = z;
                return;
            }
        }
        if(mFloat3ParamCount >= 8)
            return;
        mFloat3Params[mFloat3ParamCount].name = name;
        mFloat3Params[mFloat3ParamCount].value[0] = x;
        mFloat3Params[mFloat3ParamCount].value[1] = y;
        mFloat3Params[mFloat3ParamCount].value[2] = z;
        mFloat3ParamCount++;
    }
    void setFloat4(const char* name, const float* value)
    {
        for (int i = 0; i < mFloat4ParamCount; ++i)
        {
            if(mFloat4Params[i].name == name)
            {
                mFloat4Params[i].value[0] = value[0];
                mFloat4Params[i].value[1] = value[1];
                mFloat4Params[i].value[2] = value[2];
                mFloat4Params[i].value[3] = value[3];
                return;
            }
        }
        if(mFloat4ParamCount >= 8)
            return;
        mFloat4Params[mFloat4ParamCount].name = name;
        mFloat4Params[mFloat4ParamCount].value[0] = value[0];
        mFloat4Params[mFloat4ParamCount].value[1] = value[1];
        mFloat4Params[mFloat4ParamCount].value[2] = value[2];
        mFloat4Params[mFloat4ParamCount].value[3] = value[3];
        mFloat4ParamCount++;
    }
    void setFloat4(const char* name, const float& x, const float& y, const float& z, const float& w)
    {
        for (int i = 0; i < mFloat4ParamCount; ++i)
        {
            if(mFloat4Params[i].name == name)
            {
                mFloat4Params[i].value[0] = x;
                mFloat4Params[i].value[1] = y;
                mFloat4Params[i].value[2] = z;
                mFloat4Params[i].value[3] = w;
                return;
            }
        }
        if(mFloat4ParamCount >= 8)
            return;
        mFloat4Params[mFloat4ParamCount].name = name;
        mFloat4Params[mFloat4ParamCount].value[0] = x;
        mFloat4Params[mFloat4ParamCount].value[1] = y;
        mFloat4Params[mFloat4ParamCount].value[2] = z;
        mFloat4Params[mFloat4ParamCount].value[3] = w;
        mFloat4ParamCount++;
    }
    void setMat4(const char* name, const float* value)
    {
        for (int i = 0; i < mMat4ParamCount; ++i)
        {
            if(mMat4Params[i].name == name)
            {
                memcpy(mMat4Params[i].value, value, sizeof(float) * 16);
                return;
            }
        }
        if(mMat4ParamCount >= 8)
            return;
        mMat4Params[mMat4ParamCount].name = name;
        memcpy(mMat4Params[mMat4ParamCount].value, value, sizeof(float) * 16);
        mMat4ParamCount++;
    }
    void setSampler(const char* name, GfxTexture* texture)
    {
        for (int i = 0; i < mSampler2DParamCount; ++i)
        {
            if(mSampler2DParams[i].name == name)
            {
                mSampler2DParams[i].texture = texture;
                return;
            }
        }
        if(mSampler2DParamCount >= 8)
            return;
        mSampler2DParams[mSampler2DParamCount].name = name;
        mSampler2DParams[mSampler2DParamCount].texture = texture;
        mSampler2DParamCount++;
    }
protected:
    GLuint mHandle;
    FloatParam mFloatParams[8] = {};
    int mFloatParamCount = 0;
    Float2Param mFloat2Params[8] = {};
    int mFloat2ParamCount = 0;
    Float3Param mFloat3Params[8] = {};
    int mFloat3ParamCount = 0;
    Float4Param mFloat4Params[8] = {};
    int mFloat4ParamCount = 0;
    Mat4Param mMat4Params[8] = {};
    int mMat4ParamCount = 0;
    Sampler2DParam mSampler2DParams[8] = {};
    int mSampler2DParamCount = 0;
};

EFFECTS_NAMESPACE_END