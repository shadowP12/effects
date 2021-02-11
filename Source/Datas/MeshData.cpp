#include "MeshData.h"
#include <algorithm>
#include <assert.h>
EFFECTS_NAMESPACE_BEGIN

bool VertexAttribute::operator== (const VertexAttribute& rhs) const
{
	if (mType != rhs.mType || mOffset != rhs.mOffset || mSemantic != rhs.mSemantic)
		return false;

	return true;
}

bool VertexAttribute::operator!= (const VertexAttribute& rhs) const
{
	return !(*this == rhs);
}

uint32_t VertexAttribute::getTypeSize(VertexType type)
{
	switch (type)
	{
	case VERTEX_TYPE_COLOR:
	case VERTEX_TYPE_COLOR_ABGR:
	case VERTEX_TYPE_COLOR_ARGB:
		return sizeof(float) * 4;
	case VERTEX_TYPE_UBYTE4_NORM:
		return sizeof(uint32_t);
	case VERTEX_TYPE_FLOAT1:
		return sizeof(float);
	case VERTEX_TYPE_FLOAT2:
		return sizeof(float) * 2;
	case VERTEX_TYPE_FLOAT3:
		return sizeof(float) * 3;
	case VERTEX_TYPE_FLOAT4:
		return sizeof(float) * 4;
	case VERTEX_TYPE_USHORT1:
		return sizeof(uint16_t);
	case VERTEX_TYPE_USHORT2:
		return sizeof(uint16_t) * 2;
	case VERTEX_TYPE_USHORT4:
		return sizeof(uint16_t) * 4;
	case VERTEX_TYPE_SHORT1:
		return sizeof(int16_t);
	case VERTEX_TYPE_SHORT2:
		return sizeof(int16_t) * 2;
	case VERTEX_TYPE_SHORT4:
		return sizeof(int16_t) * 4;
	case VERTEX_TYPE_UINT1:
		return sizeof(uint32_t);
	case VERTEX_TYPE_UINT2:
		return sizeof(uint32_t) * 2;
	case VERTEX_TYPE_UINT3:
		return sizeof(uint32_t) * 3;
	case VERTEX_TYPE_UINT4:
		return sizeof(uint32_t) * 4;
	case VERTEX_TYPE_INT4:
		return sizeof(int32_t) * 4;
	case VERTEX_TYPE_INT1:
		return sizeof(int32_t);
	case VERTEX_TYPE_INT2:
		return sizeof(int32_t) * 2;
	case VERTEX_TYPE_INT3:
		return sizeof(int32_t) * 3;
	case VERTEX_TYPE_UBYTE4:
		return sizeof(uint8_t) * 4;
	default:
		break;
	}

	return 0;
}

uint16_t VertexAttribute::getTypeCount(VertexType type)
{
	switch (type)
	{
	case VERTEX_TYPE_COLOR:
	case VERTEX_TYPE_COLOR_ABGR:
	case VERTEX_TYPE_COLOR_ARGB:
		return 4;
	case VERTEX_TYPE_FLOAT1:
	case VERTEX_TYPE_SHORT1:
	case VERTEX_TYPE_USHORT1:
	case VERTEX_TYPE_INT1:
	case VERTEX_TYPE_UINT1:
		return 1;
	case VERTEX_TYPE_FLOAT2:
	case VERTEX_TYPE_SHORT2:
	case VERTEX_TYPE_USHORT2:
	case VERTEX_TYPE_INT2:
	case VERTEX_TYPE_UINT2:
		return 2;
	case VERTEX_TYPE_FLOAT3:
	case VERTEX_TYPE_INT3:
	case VERTEX_TYPE_UINT3:
		return 3;
	case VERTEX_TYPE_FLOAT4:
	case VERTEX_TYPE_SHORT4:
	case VERTEX_TYPE_USHORT4:
	case VERTEX_TYPE_INT4:
	case VERTEX_TYPE_UINT4:
	case VERTEX_TYPE_UBYTE4:
	case VERTEX_TYPE_UBYTE4_NORM:
		return 4;
	default:
		break;
	}
	//todo:Exception
	return 0;
}

VertexLayout::VertexLayout()
{
    mLayout = 0;
}

VertexLayout::VertexLayout(const uint32_t& layout)
{
    mLayout = layout;
	if ((layout & (uint32_t)SEMANTIC_POSITION) != 0)
		addVertexAttribute(VERTEX_TYPE_FLOAT3, SEMANTIC_POSITION);

	if ((layout & (uint32_t)SEMANTIC_NORMAL) != 0)
		addVertexAttribute(VERTEX_TYPE_FLOAT3, SEMANTIC_NORMAL);

    if ((layout & (uint32_t)SEMANTIC_COLOR) != 0)
        addVertexAttribute(VERTEX_TYPE_COLOR, SEMANTIC_COLOR);

	if ((layout & (uint32_t)SEMANTIC_TANGENT) != 0)
		addVertexAttribute(VERTEX_TYPE_FLOAT3, SEMANTIC_TANGENT);

	if ((layout & (uint32_t)SEMANTIC_BITANGENT) != 0)
		addVertexAttribute(VERTEX_TYPE_FLOAT3, SEMANTIC_BITANGENT);

    if ((layout & (uint32_t)SEMANTIC_JOINTS) != 0)
        addVertexAttribute(VERTEX_TYPE_INT4, SEMANTIC_JOINTS);

    if ((layout & (uint32_t)SEMANTIC_WEIGHTS) != 0)
        addVertexAttribute(VERTEX_TYPE_FLOAT4, SEMANTIC_WEIGHTS);

    if ((layout & (uint32_t)SEMANTIC_TEXCOORD0) != 0)
        addVertexAttribute(VERTEX_TYPE_FLOAT2, SEMANTIC_TEXCOORD0);

    if ((layout & (uint32_t)SEMANTIC_TEXCOORD1) != 0)
        addVertexAttribute(VERTEX_TYPE_FLOAT2, SEMANTIC_TEXCOORD1);

    if ((layout & (uint32_t)SEMANTIC_TEXCOORD2) != 0)
        addVertexAttribute(VERTEX_TYPE_FLOAT2, SEMANTIC_TEXCOORD2);

    if ((layout & (uint32_t)SEMANTIC_TEXCOORD3) != 0)
        addVertexAttribute(VERTEX_TYPE_FLOAT2, SEMANTIC_TEXCOORD3);

    if ((layout & (uint32_t)SEMANTIC_TEXCOORD4) != 0)
        addVertexAttribute(VERTEX_TYPE_FLOAT2, SEMANTIC_TEXCOORD4);

    if ((layout & (uint32_t)SEMANTIC_TEXCOORD5) != 0)
        addVertexAttribute(VERTEX_TYPE_FLOAT2, SEMANTIC_TEXCOORD5);

    if ((layout & (uint32_t)SEMANTIC_TEXCOORD6) != 0)
        addVertexAttribute(VERTEX_TYPE_FLOAT2, SEMANTIC_TEXCOORD6);

    if ((layout & (uint32_t)SEMANTIC_TEXCOORD7) != 0)
        addVertexAttribute(VERTEX_TYPE_FLOAT2, SEMANTIC_TEXCOORD5);

    if ((layout & (uint32_t)SEMANTIC_TEXCOORD8) != 0)
        addVertexAttribute(VERTEX_TYPE_FLOAT2, SEMANTIC_TEXCOORD6);
}

VertexLayout::~VertexLayout()
{
}

void VertexLayout::addVertexAttribute(VertexType type, VertexSemantic semantic)
{
	clearIfItExists(type, semantic);

	VertexAttribute newAttribute(type, semantic);

	uint32_t insertToIndex = (uint32_t)mAttributes.size();

    mAttributes.insert(mAttributes.begin() + insertToIndex, newAttribute);
}

bool VertexLayout::hasVertexAttribute(VertexSemantic semantic)
{
	auto findIter = std::find_if(mAttributes.begin(), mAttributes.end(),
		[semantic](const VertexAttribute& x)
		{
			return x.getSemantic() == semantic;
		});

	if (findIter != mAttributes.end())
	{
		return true;
	}
	return false;
}

void VertexLayout::clearIfItExists(VertexType type, VertexSemantic semantic)
{
	auto findIter = std::find_if(mAttributes.begin(), mAttributes.end(),
		[semantic](const VertexAttribute& x)
		{
			return x.getSemantic() == semantic;
		});

	if (findIter != mAttributes.end())
	{
        mAttributes.erase(findIter);
	}
}

uint32_t VertexLayout::getVertexAttributeSize(VertexSemantic semantic)
{
	for (auto& attribute : mAttributes)
	{
		if (attribute.getSemantic() == semantic)
			return attribute.getSize();
	}

	return -1;
}

uint32_t VertexLayout::getVertexAttributeOffset(VertexSemantic semantic)
{
	uint32_t offset = 0;
	for (auto& attribute : mAttributes)
	{
		if (attribute.getSemantic() == semantic)
			break;

		offset += attribute.getSize();
	}

	return offset;
}

uint32_t VertexLayout::getVertexAttributeStride()
{
	uint32_t stride = 0;
	for (uint32_t i = 0; i < mAttributes.size(); i++)
	{
		stride += mAttributes[i].getSize();
	}

	return stride;
}

MeshData::MeshData(uint32_t numVertices, uint32_t numIndices, VertexLayout* vertexLayout)
	:mNumVertices(numVertices), mNumIndices(numIndices), mVertexLayout(vertexLayout)
{
	// �����ڴ�ռ�
	uint32_t totalSize = getStreamSize() + getIndexBufferSize();
	mData = new uint8_t[totalSize];
}

MeshData::~MeshData()
{
	if (mData)
	    SAFE_DELETE_ARRAY(mData);
	if(mVertexLayout)
	    SAFE_DELETE(mVertexLayout)
}

void MeshData::setIndexes(void* data, uint32_t size)
{
	uint8_t* dst = mData;
	uint8_t* src = (uint8_t*)data;

	memcpy(dst, src, size);
}

void MeshData::setAttribute(VertexSemantic semantic, void* data, uint32_t size)
{
	assert(data != nullptr);

	if (!mVertexLayout->hasVertexAttribute(semantic))
		return;

	uint32_t attributeSize = mVertexLayout->getVertexAttributeSize(semantic);
	uint32_t totalSize = attributeSize * mNumVertices;

	if (totalSize != size)
		return;

	uint32_t offset = mVertexLayout->getVertexAttributeOffset(semantic);
	uint32_t stride = mVertexLayout->getVertexAttributeStride();

	uint8_t* dst = mData + mNumIndices * sizeof(uint32_t) + offset;
	uint8_t* src = (uint8_t*)data;
	for (uint32_t i = 0; i < mNumVertices; i++)
	{
		memcpy(dst, src, attributeSize);
		dst += stride;
		src += attributeSize;
	}
}

uint8_t* MeshData::getIndices()
{
	return mData;
}

uint8_t* MeshData::getVertices()
{
	return mData + getIndexBufferSize();
}

void MeshData::getAttribute(VertexSemantic semantic, void* data, uint32_t size)
{
	if (!mVertexLayout->hasVertexAttribute(semantic))
		return ;

	uint32_t attributeSize = mVertexLayout->getVertexAttributeSize(semantic);
	uint32_t totalSize = attributeSize * mNumVertices;

	if (totalSize != size)
		return ;

	uint32_t offset = mVertexLayout->getVertexAttributeOffset(semantic);
	uint32_t stride = mVertexLayout->getVertexAttributeStride();

	uint8_t* src = mData + offset;
	uint8_t* dst = (uint8_t*)data;
	for (uint32_t i = 0; i < mNumVertices; i++)
	{
		memcpy(dst, src, attributeSize);
		dst += attributeSize;
		src += stride;
	}
}

uint32_t MeshData::getVertexBufferSize() const
{
    return mNumVertices * mVertexLayout->getVertexAttributeStride();
}

uint32_t MeshData::getIndexBufferSize() const
{
	return mNumIndices * sizeof(uint32_t);
}

uint32_t MeshData::getVertexStrideSize() const
{
    return mVertexLayout->getVertexAttributeStride();
}

uint32_t MeshData::getStreamSize() const
{
	return mNumVertices * mVertexLayout->getVertexAttributeStride();
}

EFFECTS_NAMESPACE_END
