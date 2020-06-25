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
    uint32_t magFilter = GL_LINEAR;
    uint32_t minFilter = GL_LINEAR;
    uint32_t wrapS = GL_CLAMP_TO_EDGE;
    uint32_t wrapT = GL_CLAMP_TO_EDGE;
    uint32_t wrapR = GL_CLAMP_TO_EDGE;
} GfxSamplerDesc;

typedef struct GfxSampler
{
    uint32_t magFilter;
    uint32_t minFilter;
    uint32_t wrapS;
    uint32_t wrapT;
    uint32_t wrapR;
} GfxSampler;

    GfxSampler* createGfxSampler(const GfxSamplerDesc& desc);
    void setGfxSamplerFilter(GfxSampler* sampler, uint32_t min, uint32_t mag);
    void setGfxSamplerWrap(GfxSampler* sampler, uint32_t s, uint32_t t, uint32_t r);
    void destroyGfxSampler(GfxSampler* sampler);

typedef struct GfxTextureDesc
{
    uint32_t width;
    uint32_t height;
    uint32_t depth = 1;
    uint32_t arraySize = 1;
    uint32_t format;
    uint32_t internalFormat;
    uint32_t componentType;
    bool mipmap = false;
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
        bool mipmap = false;
    } GfxTexture;

    GfxTexture* createGfxTexture(const GfxTextureDesc& desc);
    void destroyGfxTexture(GfxTexture* tex);
    void writeGfxTextureData(const GfxTexture* tex, void* data, uint32_t arraySize = 1, uint32_t depth = 1, uint32_t level = 0);
    void writeGfxTextureData(const GfxTexture* tex, void* data, uint32_t width, uint32_t height, uint32_t arraySize = 1, uint32_t depth = 1, uint32_t level = 0);
    void setGfxTextureSampler(const GfxTexture* tex, const GfxSampler* sampler);

    typedef struct GfxRenderbufferDesc
    {
        uint32_t width;
        uint32_t height;
        uint32_t internalformat;
    } GfxRenderbufferDesc;

    typedef struct GfxRenderbuffer
    {
        uint32_t handle;
        uint32_t width;
        uint32_t height;
        uint32_t internalformat;
    } GfxRenderbuffer;

    GfxRenderbuffer* createGfxRenderbuffer(const GfxRenderbufferDesc& desc);
    void destroyGfxRenderbuffer(GfxRenderbuffer* renderbuffer);

    typedef struct GfxFramebufferDesc
    {
        GfxTexture* targets[8] = {nullptr};
        GfxRenderbuffer* depthBuffer = nullptr;
    } GfxFramebufferDesc;

    typedef struct GfxFramebuffer
    {
        GLuint handle;
        GfxTexture* tatgets[8];
        GfxRenderbuffer* depthBuffer = nullptr;
    } GfxFramebuffer;

    GfxFramebuffer* createGfxFramebuffer(const GfxFramebufferDesc& desc);
    GfxFramebuffer* createGfxFramebuffer();
    void attachGfxFramebufferTexture(GfxFramebuffer* fb, uint32_t idx, GfxTexture* texture, uint32_t level = 0);
    void attachGfxFramebufferCubeMap(GfxFramebuffer* fb, uint32_t idx, uint32_t face,  GfxTexture* texture, uint32_t level = 0);
    void attachGfxFramebufferRenderBuffer(GfxFramebuffer* fb, GfxRenderbuffer* renderbuffer);
    void destroyGfxFramebuffer(GfxFramebuffer* fb);
    void bindGfxFramebuffer(const GfxFramebuffer* fb);
    void unbindGfxFramebuffer(const GfxFramebuffer* fb);

    typedef struct GfxProgramDesc
    {
        std::string vertSource;
        std::string fragSource;
        std::string define;
    } GfxProgramDesc;

    typedef struct GfxProgram
    {
        typedef struct FloatParam
        {
            std::string name;
            float value;
        } FloatParam;

        typedef struct Float2Param
        {
            std::string name;
            float value[2];
        } Float2Param;

        typedef struct Float3Param
        {
            std::string name;
            float value[3];
        } Float3Param;

        typedef struct Float4Param
        {
            std::string name;
            float value[4];
        } Float4Param;

        typedef struct Mat4Param
        {
            std::string name;
            float value[16];
        } Mat4Param;

        typedef struct Sampler2DParam
        {
            std::string name;
            GfxTexture* texture;
        } Sampler2DParam;

        typedef struct SamplerCubeMapParam
        {
            std::string name;
            GfxTexture* texture;
        } SamplerCubeMapParam;

        GLuint handle;
        FloatParam floatParams[8] = {};
        int floatParamCount = 0;
        Float2Param float2Params[8] = {};
        int float2ParamCount = 0;
        Float3Param float3Params[8] = {};
        int float3ParamCount = 0;
        Float4Param float4Params[8] = {};
        int float4ParamCount = 0;
        Mat4Param mat4Params[8] = {};
        int mat4ParamCount = 0;
        Sampler2DParam sampler2DParams[8] = {};
        int sampler2DParamCount = 0;
        SamplerCubeMapParam samplerCubeMapParams[8] = {};
        int samplerCubeMapParamCount = 0;
    } GfxProgram;

    GfxProgram* createGfxProgram(const GfxProgramDesc& desc);
    void destroyGfxProgram(GfxProgram* program);
    void setGfxProgramFloat(GfxProgram* program, const char* name, float value);
    void setGfxProgramFloat2(GfxProgram* program, const char* name, float x, float y);
    void setGfxProgramFloat2(GfxProgram* program, const char* name, const float* value);
    void setGfxProgramFloat3(GfxProgram* program, const char* name, const float* value);
    void setGfxProgramFloat3(GfxProgram* program, const char* name, float x, float y, float z);
    void setGfxProgramFloat4(GfxProgram* program, const char* name, const float* value);
    void setGfxProgramFloat4(GfxProgram* program, const char* name, float x, float y, float z, float w);
    void setGfxProgramMat4(GfxProgram* program, const char* name, const float* value);
    void setGfxProgramSampler(GfxProgram* program, const char* name, GfxTexture* texture);
    void setGfxProgramCubeMapSampler(GfxProgram* program, const char* name, GfxTexture* texture);
    void bindGfxProgram(const GfxProgram* program);
    void unbindGfxProgram(const GfxProgram* program);
EFFECTS_NAMESPACE_END