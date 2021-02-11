#pragma once

#include "Core/Base.h"
#include "Resources/Resource.h"
#include "Core/Gfx/GfxResources.h"
#include <string>

EFFECTS_NAMESPACE_BEGIN
class PixelData;
class Texture : public Resource {
public:
    Texture(PixelData* data);
    ~Texture();
    static std::shared_ptr<Texture> create(const std::string& id, PixelData* data);
    static void remove(std::shared_ptr<Texture> texture);
    void prepareGfxData();
    void setFilter(uint32_t min, uint32_t mag);
    void setWrap(uint32_t s, uint32_t t, uint32_t r);
private:
    PixelData* mData = nullptr;
    GfxTexture* mInternelTexture = nullptr;
    GfxSampler* mSampler = nullptr;
};

EFFECTS_NAMESPACE_END