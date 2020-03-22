#include "TextureImporter.h"
#define STB_IMAGE_IMPLEMENTATIO
#include <stb_image.h>
EFFECTS_NAMESPACE_BEGIN

unsigned char* loadImage(const char* filePath, int& width, int& height, int& channel)
{
    unsigned char* pixels = stbi_load(filePath, &width, &height, &channel, STBI_rgb_alpha);
    return pixels;
}

EFFECTS_NAMESPACE_END