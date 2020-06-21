#pragma once
#include "../Core/Base.h"
#include <string>
EFFECTS_NAMESPACE_BEGIN

unsigned char* loadImage(const char* filePath, int& width, int& height, int& channel, bool flip = false);

float* loadFloatImage(const char* filePath, int& width, int& height, int& channel, bool flip = false);

EFFECTS_NAMESPACE_END
