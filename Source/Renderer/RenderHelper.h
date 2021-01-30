#pragma once

#include "Core/Base.h"
#include "Core/Gfx/GfxResources.h"
#include <string>
EFFECTS_NAMESPACE_BEGIN

GfxTexture* loadTexture(std::string filePath);

GfxProgram* loadProgram(std::string vsFilePath, std::string fsFilePath);

EFFECTS_NAMESPACE_END
