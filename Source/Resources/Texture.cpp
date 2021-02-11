#include "Resources/Texture.h"
#include "Resources/ResourceManager.h"
#include "Datas/PixelData.h"
#include "Core/Utility/UUID.h"

EFFECTS_NAMESPACE_BEGIN
Texture::Texture(PixelData *data) {
    mData = data;
}
Texture::~Texture() {
    SAFE_DELETE(mData);
    destroyGfxTexture(mInternelTexture);
    destroyGfxSampler(mSampler);
}

std::shared_ptr<Texture> Texture::create(const std::string& id, PixelData* data) {
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(data);
    texture->mId = id;
    texture->mUUID = generateUUID();
    texture->mType = ResourceType::TEXTURE;
    ResourceManager::instance().addRes(texture);
    return texture;
}

void Texture::remove(std::shared_ptr<Texture> texture) {
    ResourceManager::instance().removeRes(texture);
}

void Texture::prepareGfxData() {
    GfxTextureDesc textureDesc;
    textureDesc.width = mData->getWidth();
    textureDesc.height = mData->getHeight();
    textureDesc.depth = mData->getDepth();
    textureDesc.arraySize = mData->getArray();
    textureDesc.componentType = getGLComponentType(mData->getFormat());
    textureDesc.format = getGLFormat(mData->getFormat());
    textureDesc.internalFormat = mData->getFormat();
    mInternelTexture = createGfxTexture(textureDesc);
    for (int i = 0; i < mData->getArray(); ++i) {
        uint8_t* data = mData->getData(i);
        writeGfxTextureData(mInternelTexture, data, i);
    }

    GfxSamplerDesc samplerDesc;
    mSampler = createGfxSampler(samplerDesc);
    setGfxTextureSampler(mInternelTexture, mSampler);
}

void Texture::setFilter(uint32_t min, uint32_t mag) {
    setGfxSamplerFilter(mSampler, min, mag);
    setGfxTextureSampler(mInternelTexture, mSampler);
}

void Texture::setWrap(uint32_t s, uint32_t t, uint32_t r) {
    setGfxSamplerWrap(mSampler, s, t, r);
    setGfxTextureSampler(mInternelTexture, mSampler);
}

EFFECTS_NAMESPACE_END