#include "MeshData.h"
#include <algorithm>
#include <assert.h>
EFFECTS_NAMESPACE_BEGIN

bool MeshAttribute::operator== (const MeshAttribute& rhs) const
{
	if (mType != rhs.mType || mOffset != rhs.mOffset || mSemantic != rhs.mSemantic)
		return false;

	return true;
}

bool MeshAttribute::operator!= (const MeshAttribute& rhs) const
{
	return !(*this == rhs);
}

uint32_t MeshAttribute::getTypeSize(MeshAttributeType type)
{
	switch (type)
	{
	case MAT_COLOR:
	case MAT_COLOR_ABGR:
	case MAT_COLOR_ARGB:
		return sizeof(float) * 4;
	case MAT_UBYTE4_NORM:
		return sizeof(uint32_t);
	case MAT_FLOAT1:
		return sizeof(float);
	case MAT_FLOAT2:
		return sizeof(float) * 2;
	case MAT_FLOAT3:
		return sizeof(float) * 3;
	case MAT_FLOAT4:
		return sizeof(float) * 4;
	case MAT_USHORT1:
		return sizeof(uint16_t);
	case MAT_USHORT2:
		return sizeof(uint16_t) * 2;
	case MAT_USHORT4:
		return sizeof(uint16_t) * 4;
	case MAT_SHORT1:
		return sizeof(int16_t);
	case MAT_SHORT2:
		return sizeof(int16_t) * 2;
	case MAT_SHORT4:
		return sizeof(int16_t) * 4;
	case MAT_UINT1:
		return sizeof(uint32_t);
	case MAT_UINT2:
		return sizeof(uint32_t) * 2;
	case MAT_UINT3:
		return sizeof(uint32_t) * 3;
	case MAT_UINT4:
		return sizeof(uint32_t) * 4;
	case MAT_INT4:
		return sizeof(int32_t) * 4;
	case MAT_INT1:
		return sizeof(int32_t);
	case MAT_INT2:
		return sizeof(int32_t) * 2;
	case MAT_INT3:
		return sizeof(int32_t) * 3;
	case MAT_UBYTE4:
		return sizeof(uint8_t) * 4;
	default:
		break;
	}

	return 0;
}

uint16_t MeshAttribute::getTypeCount(MeshAttributeType type)
{
	switch (type)
	{
	case MAT_COLOR:
	case MAT_COLOR_ABGR:
	case MAT_COLOR_ARGB:
		return 4;
	case MAT_FLOAT1:
	case MAT_SHORT1:
	case MAT_USHORT1:
	case MAT_INT1:
	case MAT_UINT1:
		return 1;
	case MAT_FLOAT2:
	case MAT_SHORT2:
	case MAT_USHORT2:
	case MAT_INT2:
	case MAT_UINT2:
		return 2;
	case MAT_FLOAT3:
	case MAT_INT3:
	case MAT_UINT3:
		return 3;
	case MAT_FLOAT4:
	case MAT_SHORT4:
	case MAT_USHORT4:
	case MAT_INT4:
	case MAT_UINT4:
	case MAT_UBYTE4:
	case MAT_UBYTE4_NORM:
		return 4;
	default:
		break;
	}
	//todo:Exception
	return 0;
}

MeshDescription::MeshDescription()
{
}

MeshDescription::MeshDescription(const MeshAttributeLayout& layout)
{
	int32_t intLayout = (int32_t)layout;

	if ((intLayout & (int32_t)MAL_POSITION) != 0)
		addMeshAttribute(MAT_FLOAT3, MAS_POSITION);

	if ((intLayout & (int32_t)MAL_NORMAL) != 0)
		addMeshAttribute(MAT_FLOAT3, MAS_NORMAL);

	if ((intLayout & (int32_t)MAL_TANGENT) != 0)
		addMeshAttribute(MAT_FLOAT3, MAS_TANGENT);

	if ((intLayout & (int32_t)MAL_TEXCOORD0) != 0)
		addMeshAttribute(MAT_FLOAT2, MAS_TEXCOORD);

	if ((intLayout & (int32_t)MAL_COLOR) != 0)
		addMeshAttribute(MAT_COLOR, MAS_COLOR);

	if ((intLayout & (int32_t)MAL_BLEND_WEIGHTS) != 0)
	{
		addMeshAttribute(MAT_INT4, MAS_BLEND_INDICES);
		addMeshAttribute(MAT_FLOAT4, MAS_BLEND_WEIGHTS);
	}
}

MeshDescription::~MeshDescription()
{
}

void MeshDescription::addMeshAttribute(MeshAttributeType type, MeshAttributeSemantic semantic)
{
	clearIfItExists(type, semantic);

	MeshAttribute newAttribute(type, semantic);

	uint32_t insertToIndex = (uint32_t)mAttributes.size();

	mAttributes.insert(mAttributes.begin() + insertToIndex, newAttribute);
}

bool MeshDescription::hasMeshAttribute(MeshAttributeSemantic semantic)
{
	auto findIter = std::find_if(mAttributes.begin(), mAttributes.end(),
		[semantic](const MeshAttribute& x)
		{
			return x.getSemantic() == semantic;
		});

	if (findIter != mAttributes.end())
	{
		return true;
	}
	return false;
}

void MeshDescription::clearIfItExists(MeshAttributeType type, MeshAttributeSemantic semantic)
{
	auto findIter = std::find_if(mAttributes.begin(), mAttributes.end(),
		[semantic](const MeshAttribute& x)
		{
			return x.getSemantic() == semantic;
		});

	if (findIter != mAttributes.end())
	{
		mAttributes.erase(findIter);
	}
}

uint32_t MeshDescription::getMeshAttributeSize(MeshAttributeSemantic semantic)
{
	for (auto& attribute : mAttributes)
	{
		if (attribute.getSemantic() == semantic)
			return attribute.getSize();
	}

	return -1;
}

uint32_t MeshDescription::getMeshAttributeOffset(MeshAttributeSemantic semantic)
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

uint32_t MeshDescription::getMeshAttributeStride()
{
	uint32_t stride = 0;
	for (uint32_t i = 0; i < mAttributes.size(); i++)
	{
		stride += mAttributes[i].getSize();
	}

	return stride;
}

MeshData::MeshData(uint32_t numVertices, uint32_t numIndices, MeshDescription* desc)
	:mNumVertices(numVertices), mNumIndices(numIndices), mDesc(desc)
{
	// ·ÖÅäÄÚ´æ¿Õ¼ä
	uint32_t totalSize = getStreamSize() + getIndexBufferSize();
	mData = new uint8_t[totalSize];
}

MeshData::~MeshData()
{
	if (mData)
		delete[] mData;
}

void MeshData::setIndexes(void* data, uint32_t size)
{
	uint8_t* dst = mData;
	uint8_t* src = (uint8_t*)data;
	memcpy(dst, src, size);
}

void MeshData::setAttribute(MeshAttributeSemantic semantic, void* data, uint32_t size)
{
	assert(data != nullptr);

	if (!mDesc->hasMeshAttribute(semantic))
		return;

	uint32_t attributeSize = mDesc->getMeshAttributeSize(semantic);
	uint32_t totalSize = attributeSize * mNumVertices;

	if (totalSize != size)
		return;

	uint32_t offset = mDesc->getMeshAttributeOffset(semantic);
	uint32_t stride = mDesc->getMeshAttributeStride();

	uint8_t* dst = mData + offset;
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

void MeshData::getAttribute(MeshAttributeSemantic semantic, void* data, uint32_t size)
{
	if (!mDesc->hasMeshAttribute(semantic))
		return ;

	uint32_t attributeSize = mDesc->getMeshAttributeSize(semantic);
	uint32_t totalSize = attributeSize * mNumVertices;

	if (totalSize != size)
		return ;

	uint32_t offset = mDesc->getMeshAttributeOffset(semantic);
	uint32_t stride = mDesc->getMeshAttributeStride();

	uint8_t* src = mData + offset;
	uint8_t* dst = (uint8_t*)data;
	for (uint32_t i = 0; i < mNumVertices; i++)
	{
		memcpy(dst, src, attributeSize);
		dst += attributeSize;
		src += stride;
	}
}

uint32_t MeshData::getIndexBufferSize() const
{
	return mNumIndices * sizeof(uint32_t);
}

uint32_t MeshData::getStreamSize() const
{
	return mNumVertices * mDesc->getMeshAttributeStride();
}

EFFECTS_NAMESPACE_END
