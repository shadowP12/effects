#include "TextureImporter.h"
#define STB_IMAGE_IMPLEMENTATIO
#include <stb_image.h>
EFFECTS_NAMESPACE_BEGIN

unsigned char* loadImage(const char* filePath, int& width, int& height, int& channel, bool flip)
{
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* pixels = stbi_load(filePath, &width, &height, &channel, STBI_rgb_alpha);
    return pixels;
}

float* loadFloatImage(const char* filePath, int& width, int& height, int& channel, bool flip)
{
    stbi_set_flip_vertically_on_load(flip);
    float* pixels = stbi_loadf(filePath, &width, &height, &channel, 0);
    return pixels;
}

EFFECTS_NAMESPACE_END