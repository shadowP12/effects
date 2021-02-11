#include "PixelData.h"

EFFECTS_NAMESPACE_BEGIN
PixelData::PixelData(const PixelDataDesc& desc) {
    mWidth = desc.width;
    mHeight = desc.height;
    mDepth = desc.depth;
    mArray = desc.array;
    mFormat = desc.format;
    for (int i = 0; i < mArray; ++i) {
        uint8_t* data = new uint8_t[mWidth * mHeight * mDepth * getGLFormatStride(mFormat)];
    }
}

PixelData::~PixelData() {
    for (int i = 0; i < mDatas.size(); ++i) {
        SAFE_DELETE_ARRAY(mDatas[i]);
    }
    mDatas.clear();
}

void PixelData::setData(uint32_t i, void* data, uint32_t size) {
    uint8_t* dst = mDatas[i];
    uint8_t* src = (uint8_t*)data;
    memcpy(dst, src, size);
}

EFFECTS_NAMESPACE_END