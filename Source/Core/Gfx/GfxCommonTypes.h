#pragma once
#include "../Base.h"
#include "Gfx.h"
EFFECTS_NAMESPACE_BEGIN

enum class GfxPixelFormat
{
    A8,
    L8,
    LA8,

    R8,
    R8SN,
    R8UI,
    R8I,
    R16F,
    R16UI,
    R16I,
    R32F,
    R32UI,
    R32I,

    RG8,
    RG8SN,
    RG8UI,
    RG8I,
    RG16F,
    RG16UI,
    RG16I,
    RG32F,
    RG32UI,
    RG32I,

    RGB8,
    SRGB8,
    RGB8SN,
    RGB8UI,
    RGB8I,
    RGB16F,
    RGB16UI,
    RGB16I,
    RGB32F,
    RGB32UI,
    RGB32I,

    RGBA8,
    SRGB8_A8,
    RGBA8SN,
    RGBA8UI,
    RGBA8I,
    RGBA16F,
    RGBA16UI,
    RGBA16I,
    RGBA32F,
    RGBA32UI,
    RGBA32I,

    // Depth-Stencil Format
    D16,
    D16S8,
    D24,
    D24S8,
    D32F,
    D32F_S8,
};

enum GfxPixelComponentType
{
    BYTE,
    SHORT,
    INT,
    FLOAT16,
    FLOAT32
};

enum class GfxFilterType
{
    MIN,
    MAG,
    MIP
};

enum class GfxFilterOption
{
    NEAREST,
    LINEAR
};

enum class GfxAddressingMode
{
    WRAP,
    MIRROR,
    CLAMP,
    BORDER
};

EFFECTS_NAMESPACE_END