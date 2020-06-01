#include "GfxResources.h"
#include <string>
EFFECTS_NAMESPACE_BEGIN

static unsigned int gfxPixelComponentTypeToGLPixelComponentType(const GfxPixelComponentType& gfxComponentType)
{
    switch (gfxComponentType)
    {
        case GfxPixelComponentType::BYTE:
            return GL_UNSIGNED_BYTE;
        case GfxPixelComponentType::SHORT:
            return GL_SHORT;
        case GfxPixelComponentType::INT:
            return GL_INT;
        case GfxPixelComponentType::FLOAT16:
            return GL_HALF_FLOAT;
        case GfxPixelComponentType::FLOAT32:
            return GL_FLOAT;
        default:
            return GL_UNSIGNED_BYTE;
    }
}

static unsigned int gfxPixelFormatToGLPixelInternalFormat(const GfxPixelFormat& gfxFormat)
{
    switch (gfxFormat)
    {
        case GfxPixelFormat::A8:
            return GL_ALPHA;
        case GfxPixelFormat::L8:
            return GL_LUMINANCE;
        case GfxPixelFormat::LA8:
            return GL_LUMINANCE_ALPHA;
        case GfxPixelFormat::RGB8:
            return GL_RGB;
        case GfxPixelFormat::RGB16F:
            return GL_RGB;
        case GfxPixelFormat::RGB32F:
            return GL_RGB;
        case GfxPixelFormat::RGBA8:
            return GL_RGBA;
        case GfxPixelFormat::RGBA16F:
            return GL_RGBA;
        case GfxPixelFormat::RGBA32F:
            return GL_RGBA;
        case GfxPixelFormat::D16:
            return GL_DEPTH_COMPONENT;
        case GfxPixelFormat::D16S8:
            return GL_DEPTH_STENCIL;
        case GfxPixelFormat::D24:
            return GL_DEPTH_COMPONENT;
        case GfxPixelFormat::D24S8:
            return GL_DEPTH_STENCIL;
        case GfxPixelFormat::D32F:
            return GL_DEPTH_COMPONENT;
        case GfxPixelFormat::D32F_S8:
            return GL_DEPTH_STENCIL;
        default:
            return GL_RGBA;
    }
}

static unsigned int gfxPixelFormatToGLPixelFormat(const GfxPixelFormat& gfxFormat)
{
    switch (gfxFormat)
    {
        case GfxPixelFormat::A8:
            return GL_ALPHA;
        case GfxPixelFormat::L8:
            return GL_LUMINANCE;
        case GfxPixelFormat::LA8:
            return GL_LUMINANCE_ALPHA;
        case GfxPixelFormat::RGB8:
            return GL_RGB;
        case GfxPixelFormat::RGB16F:
            return GL_RGB;
        case GfxPixelFormat::RGB32F:
            return GL_RGB;
        case GfxPixelFormat::RGBA8:
            return GL_RGBA;
        case GfxPixelFormat::RGBA16F:
            return GL_RGBA;
        case GfxPixelFormat::RGBA32F:
            return GL_RGBA;
        case GfxPixelFormat::D16:
            return GL_DEPTH_COMPONENT;
        case GfxPixelFormat::D16S8:
            return GL_DEPTH_STENCIL;
        case GfxPixelFormat::D24:
            return GL_DEPTH_COMPONENT;
        case GfxPixelFormat::D24S8:
            return GL_DEPTH_STENCIL;
        case GfxPixelFormat::D32F:
            return GL_DEPTH_COMPONENT;
        case GfxPixelFormat::D32F_S8:
            return GL_DEPTH_STENCIL;
        default:
            return GL_RGBA;
    }
}

static unsigned int gfxFilterTypeToGLFilterType(const GfxFilterType& gfxFilterType)
{
    switch (gfxFilterType)
    {
        case GfxFilterType::MAG:
            return GL_TEXTURE_MAG_FILTER;
        case GfxFilterType::MIN:
            return GL_TEXTURE_MIN_FILTER;
        default:
            return GL_TEXTURE_MAG_FILTER;
    }
}

static unsigned int gfxFilterOptionToGLFilterOption(const GfxFilterOption& gfxFilterOption)
{
    switch (gfxFilterOption)
    {
        case GfxFilterOption::NEAREST:
            return GL_NEAREST;
        case GfxFilterOption::LINEAR:
            return GL_LINEAR;
        default:
            return GL_NEAREST;
    }
}

static unsigned int gfxAddressingModeToGLAddressingMode(const GfxAddressingMode& gfxAddressingMode)
{
    switch (gfxAddressingMode)
    {
        case GfxAddressingMode::WRAP:
            return GL_REPEAT;
        case GfxAddressingMode::MIRROR:
            return GL_MIRRORED_REPEAT;
        case GfxAddressingMode::CLAMP:
            return GL_CLAMP_TO_EDGE;
        case GfxAddressingMode::BORDER:
            return GL_CLAMP_TO_BORDER;
        default:
            return GL_REPEAT;
    }
}

GfxBuffer::GfxBuffer(const GfxBufferDesc& desc)
:mSize(desc.size), mBufferUsage(desc.bufferUsage), mMemUsage(desc.memUsage), mBufferAccess(desc.bufferAccess)
{
    glGenBuffers(1, &mHandle);
    resize(mSize);
}

GfxBuffer::~GfxBuffer()
{
    glDeleteBuffers(1, &mHandle);
}

void GfxBuffer::writeData(const void* data, const int& size)
{
    if(mBufferUsage == GfxBufferUsageBit::VERTEX)
    {
        glBindBuffer(GL_ARRAY_BUFFER, mHandle);
        GLenum memUsage = mMemUsage == GfxMemoryUsageBit::DEVICE ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
        glBufferData(GL_ARRAY_BUFFER, size, data, memUsage);
    }
    else if(mBufferUsage == GfxBufferUsageBit::INDEX)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
        GLenum memUsage = mMemUsage == GfxMemoryUsageBit::DEVICE ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, memUsage);
    }
}

void GfxBuffer::writeData(const void* data, const int& offset, const int& size)
{
    if(mBufferUsage == GfxBufferUsageBit::VERTEX)
    {
        glBindBuffer(GL_ARRAY_BUFFER, mHandle);
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }
    else if(mBufferUsage == GfxBufferUsageBit::INDEX)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
    }
}

void GfxBuffer::resize(int size)
{
    if(mBufferUsage == GfxBufferUsageBit::VERTEX)
    {
        glBindBuffer(GL_ARRAY_BUFFER, mHandle);
        GLenum memUsage = mMemUsage == GfxMemoryUsageBit::DEVICE ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, memUsage);
    }
    else if(mBufferUsage == GfxBufferUsageBit::INDEX)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
        GLenum memUsage = mMemUsage == GfxMemoryUsageBit::DEVICE ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, memUsage);
    }
}
    GfxSampler* createGfxSampler(const GfxSamplerDesc& desc)
    {
        GfxSampler* sampler = new GfxSampler();
        sampler->magFilter = desc.magFilter;
        sampler->minFilter = desc.minFilter;
        sampler->wrapS = desc.wrapS;
        sampler->wrapT = desc.wrapT;
        return sampler;
    }

    void destroyGfxSampler(GfxSampler* sampler)
    {
        if(sampler)
            delete sampler;
    }

    GfxTexture* createGfxTexture(const GfxTextureDesc& desc)
    {
        GfxTexture* tex = new GfxTexture();
        tex->width = desc.width;
        tex->height = desc.height;
        tex->depth = desc.depth;
        tex->arraySize = desc.arraySize;
        tex->format = desc.format;
        tex->internalFormat = desc.internalFormat;
        tex->componentType = desc.componentType;
        glGenTextures(1, &tex->handle);
        return tex;
    }

    void destroyGfxTexture(GfxTexture* tex)
    {
        if(!tex)
        {
            glDeleteTextures(1, &tex->handle);
            delete tex;
        }
    }

    void writeGfxTextureData(const GfxTexture* tex, void* data, uint32_t arraySize, uint32_t depth)
    {
        if(tex->arraySize == 1 && tex->depth == 1)
        {
            // 2D
            glBindTexture(GL_TEXTURE_2D, tex->handle);
            glTexImage2D(GL_TEXTURE_2D, 0, tex->format, tex->width, tex->height, 0, tex->internalFormat, tex->componentType, data);
            glBindTexture(GL_TEXTURE_2D, 0);
        } else if(tex->arraySize == 6 && tex->depth == 1)
        {
            // cube
        }
    }

    void setGfxTextureSampler(const GfxTexture* tex, const GfxSampler* sampler)
    {
        glBindTexture(GL_TEXTURE_2D, tex->handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampler->magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampler->minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler->wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler->wrapT);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

GfxFramebuffer::GfxFramebuffer(const GfxFramebufferDesc& desc)
{
    for (int i = 0; i < 8; ++i)
    {
        mTatgets[i] = desc.targets[i];
    }

    glGenFramebuffers(1, &mHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
    for (int i = 0; i < 8; ++i)
    {
        if(mTatgets[i] != nullptr)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mTatgets[i]->handle, 0);
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GfxFramebuffer::~GfxFramebuffer()
{
    glDeleteFramebuffers(1, &mHandle);
}

void GfxFramebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
}

static void checkShaderCompileErrors(GLuint id)
{
    GLint success;
    GLchar info_log[1024];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(id, 1024, NULL, info_log);
        printf("shader compile error : %s \n", info_log);
    }
}

static void checkProgramCompileErrors(GLuint id)
{
    GLint success;
    GLchar info_log[1024];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 1024, NULL, info_log);
        printf("program compile error : %s \n", info_log);
    }
}

static GLuint createGpuProgram(const char* vertex_source, const char* fragment_source)
{
    const char* vertex_source_c = vertex_source;
    const char* fragment_source_c = fragment_source;

    GLuint vertex_id, fragment_id;
    vertex_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_id, 1, &vertex_source_c, NULL);
    glCompileShader(vertex_id);
    checkShaderCompileErrors(vertex_id);

    fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_id, 1, &fragment_source_c, NULL);
    glCompileShader(fragment_id);
    checkShaderCompileErrors(fragment_id);

    GLuint program_id;
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_id);
    glAttachShader(program_id, fragment_id);
    glLinkProgram(program_id);
    checkProgramCompileErrors(program_id);

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

    return program_id;
}

GfxProgram::GfxProgram(const char* vs, const char* fs)
{
    mHandle = createGpuProgram(vs, fs);
}

GfxProgram::~GfxProgram()
{
    glDeleteProgram(mHandle);
}

void GfxProgram::bind()
{
    glUseProgram(mHandle);
    for (int i = 0; i < mFloatParamCount; ++i)
    {
        glUniform1f(glGetUniformLocation(mHandle, mFloatParams[i].name.c_str()), mFloatParams[i].value);
    }

    for (int i = 0; i < mFloat2ParamCount; ++i)
    {
        glUniform2fv(glGetUniformLocation(mHandle, mFloat2Params[i].name.c_str()), 1, &mFloat2Params[i].value[0]);
    }

    for (int i = 0; i < mFloat3ParamCount; ++i)
    {
        glUniform3fv(glGetUniformLocation(mHandle, mFloat3Params[i].name.c_str()), 1, &mFloat3Params[i].value[0]);
    }

    for (int i = 0; i < mFloat4ParamCount; ++i)
    {
        glUniform4fv(glGetUniformLocation(mHandle, mFloat4Params[i].name.c_str()), 1, &mFloat4Params[i].value[0]);
    }

    for (int i = 0; i < mMat4ParamCount; ++i)
    {
        glUniformMatrix4fv(glGetUniformLocation(mHandle, mMat4Params[i].name.c_str()), 1, GL_FALSE, &mMat4Params[i].value[0]);
    }

    for (int i = 0; i < mSampler2DParamCount; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mSampler2DParams[i].texture->handle);
        glUniform1i(glGetUniformLocation(mHandle, mSampler2DParams[i].name.c_str()), i);
    }
}

EFFECTS_NAMESPACE_END