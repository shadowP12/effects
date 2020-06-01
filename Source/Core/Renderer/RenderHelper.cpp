#include "RenderHelper.h"
#include "../Utility/FileUtility.h"
#include "../../Importers/TextureImporter.h"
EFFECTS_NAMESPACE_BEGIN

GfxTexture* loadTexture(std::string filePath)
{
//    int width, height, channels;
//    unsigned char* pixels = loadImage(filePath.c_str(), width, height, channels);
//    if (!pixels)
//    {
//        return nullptr;
//    }
//
//    GfxTextureDesc textureDesc;
//    textureDesc.width = width;
//    textureDesc.height = height;
//    textureDesc.componentType = GfxPixelComponentType::BYTE;
//    textureDesc.format = GfxPixelFormat::RGBA8;
//    GfxTexture* texture = new GfxTexture(textureDesc);
//    texture->writeData(pixels);
//    delete[] pixels;
    return nullptr;
}

GfxProgram* loadProgram(std::string vsFilePath, std::string fsFilePath)
{
    std::string vs, fs;
    readFileData(vsFilePath, vs);
    readFileData(fsFilePath, fs);
    GfxProgram* program = new GfxProgram(vs.c_str(), fs.c_str());
    return program;
}

EFFECTS_NAMESPACE_END
