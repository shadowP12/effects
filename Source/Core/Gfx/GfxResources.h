#pragma once
#include "../Base.h"
#include "Gfx.h"
#include "GfxCommonTypes.h"
#include <string>
EFFECTS_NAMESPACE_BEGIN

class GfxTexture;
class GfxFramebuffer;
class GfxProgram;


struct GfxTextureDesc
{
    int width;
    int height;
    GfxPixelFormat format;
    GfxPixelComponentType componentType;
};

class GfxTexture
{
public:
    GfxTexture(const GfxTextureDesc& desc);
    ~GfxTexture();
    void writeData(void* data);
    void setFiltering(const GfxFilterType& type, const GfxFilterOption& option);
    void setAddressingMode(const GfxAddressingMode& s = GfxAddressingMode::WRAP,
            const GfxAddressingMode& t = GfxAddressingMode::WRAP,
            const GfxAddressingMode& r = GfxAddressingMode::WRAP);

protected:
    friend class GfxFramebuffer;
    friend class GfxProgram;
    GLuint mHandle;
    int mWidth;
    int mHeight;
    GfxPixelFormat mFormat;
    GfxPixelComponentType mComponentType;
};

struct GfxFramebufferDesc
{
    GfxTexture* targets[8] = {nullptr};
};

class GfxFramebuffer
{
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
        if(mFloatParamCount >= 8)
            return;
        mFloatParams[mFloatParamCount].name = name;
        mFloatParams[mFloatParamCount].value = value;
        mFloatParamCount++;
    }
    void setFloat2(const char* name, const float* value)
    {
        if(mFloat2ParamCount >= 8)
            return;
        mFloat2Params[mFloat2ParamCount].name = name;
        mFloat2Params[mFloat2ParamCount].value[0] = value[0];
        mFloat2Params[mFloat2ParamCount].value[1] = value[1];
        mFloat2ParamCount++;
    }
    void setFloat2(const char* name, const float& x, const float& y)
    {
        if(mFloat2ParamCount >= 8)
            return;
        mFloat2Params[mFloat2ParamCount].name = name;
        mFloat2Params[mFloat2ParamCount].value[0] = x;
        mFloat2Params[mFloat2ParamCount].value[1] = y;
        mFloat2ParamCount++;
    }
    void setFloat3(const char* name, const float* value)
    {
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
        if(mMat4ParamCount >= 8)
            return;
        mMat4Params[mMat4ParamCount].name = name;
        memcpy(mMat4Params[mMat4ParamCount].value, value, sizeof(float) * 16);
        mMat4ParamCount++;
    }
    void setSampler(const char* name, GfxTexture* texture)
    {
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