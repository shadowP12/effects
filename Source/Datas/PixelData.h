#pragma once
#include "Core/Base.h"
#include "Core/Gfx/Gfx.h"
#include <string>
#include <vector>
EFFECTS_NAMESPACE_BEGIN

struct PixelDataDesc {
    uint32_t width = 1;
    uint32_t height = 1;
    uint32_t depth = 1;
    uint32_t array = 1;
    uint32_t format;
};
// todo PixelSubData
class PixelData
{
public:
	PixelData(const PixelDataDesc& desc);
	~PixelData();
	uint32_t getWidth() { return mWidth; }
    uint32_t getHeight() { return mHeight; }
    uint32_t getDepth() { return mDepth; }
    uint32_t getArray() { return mArray; }
    uint32_t getFormat() { return mFormat; }
    void setData(uint32_t i, void* data, uint32_t size);
    uint8_t* getData(uint32_t i) { return mDatas[i]; }
private:
    uint32_t mWidth;
    uint32_t mHeight;
    uint32_t mDepth;
    uint32_t mArray;
    uint32_t mFormat;
    std::vector<uint8_t*> mDatas;
};

EFFECTS_NAMESPACE_END