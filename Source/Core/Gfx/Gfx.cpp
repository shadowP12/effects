#include "Core/Gfx/Gfx.h"
#include <assert.h>

unsigned int getGLFormatStride(unsigned int format) {
    switch (format) {
        case GL_RGBA32F:
            return 16;

        case GL_RGB32F:
            return 12;

        case GL_RGBA16F:
            return 8;

        case GL_RGB16F:
            return 6;

        case GL_RGBA8:
        case GL_RGBA:
            return 4;

        case GL_RGB8:
        case GL_RGB:
            return 3;

        case GL_LUMINANCE_ALPHA:
            return 2;

        case GL_R8:
        case GL_ALPHA:
        case GL_LUMINANCE:
            return 1;

        default:
            assert(0);
            break;
    }

    return 0;
}


unsigned int getGLFormat(unsigned int format) {
    switch (format) {
        case GL_RGBA32F:
        case GL_RGBA16F:
        case GL_RGBA8:
        case GL_RGBA:
            return GL_RGBA;

        case GL_RGB32F:
        case GL_RGB16F:
        case GL_RGB8:
        case GL_RGB:
            return GL_RGB;

        case GL_R8:
            return GL_R;

        case GL_LUMINANCE_ALPHA:
            return GL_LUMINANCE_ALPHA;

        case GL_ALPHA:
            return GL_ALPHA;

        case GL_LUMINANCE:
            return GL_LUMINANCE;

        default:
            assert(0);
            break;
    }

    return GL_NONE;
}

unsigned int getGLComponentType(unsigned int format) {
    switch (format) {
        case GL_RGB16F:
        case GL_RGBA16F:
        case GL_RGB32F:
        case GL_RGBA32F:
            return GL_FLOAT;

        case GL_R8:
        case GL_RGB:
        case GL_RGB8:
        case GL_RGBA:
        case GL_RGBA8:
        case GL_ALPHA:
        case GL_LUMINANCE:
        case GL_LUMINANCE_ALPHA:
            return GL_UNSIGNED_BYTE;

        default:
            assert(0);
            break;
    }

    return GL_NONE;
}