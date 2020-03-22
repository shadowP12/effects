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

GfxTexture::GfxTexture(const GfxTextureDesc& desc)
:mWidth(desc.width), mHeight(desc.height), mFormat(desc.format), mComponentType(desc.componentType)
{
    glGenTextures(1, &mHandle);
    writeData(nullptr);
    setFiltering(GfxFilterType::MIN, GfxFilterOption::LINEAR);
    setFiltering(GfxFilterType::MAG, GfxFilterOption::LINEAR);
    setAddressingMode();
}

GfxTexture::~GfxTexture()
{
    glDeleteTextures(1, &mHandle);
}

void GfxTexture::writeData(void* data)
{
    glBindTexture(GL_TEXTURE_2D, mHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, gfxPixelFormatToGLPixelInternalFormat(mFormat),
            mWidth, mHeight, 0, gfxPixelFormatToGLPixelFormat(mFormat),
            gfxPixelComponentTypeToGLPixelComponentType(mComponentType), data);
}

void GfxTexture::setFiltering(const GfxFilterType& type, const GfxFilterOption& option)
{
    glBindTexture(GL_TEXTURE_2D, mHandle);
    glTexParameteri(GL_TEXTURE_2D, gfxFilterTypeToGLFilterType(type), gfxFilterOptionToGLFilterOption(option));
}

void GfxTexture::setAddressingMode(const GfxAddressingMode& s, const GfxAddressingMode& t, const GfxAddressingMode& r)
{
    glBindTexture(GL_TEXTURE_2D, mHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gfxAddressingModeToGLAddressingMode(s));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gfxAddressingModeToGLAddressingMode(t));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, gfxAddressingModeToGLAddressingMode(r));
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
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mTatgets[i]->mHandle, 0);
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
        glUniformMatrix4fv(glGetUniformLocation(mHandle, mMat4Params[i].name.c_str()), 1, GL_FALSE, &mMat4Params->value[0]);
    }

    for (int i = 0; i < mSampler2DParamCount; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mSampler2DParams[i].texture->mHandle);
        glUniform1i(glGetUniformLocation(mHandle, mSampler2DParams[i].name.c_str()), i);
    }
}

EFFECTS_NAMESPACE_END