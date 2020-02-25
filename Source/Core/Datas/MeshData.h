#pragma once
#include "../Base.h"
#include <vector>
EFFECTS_NAMESPACE_BEGIN

enum MeshAttributeLayout
{
	MAL_POSITION = 1 << 0,
	MAL_COLOR = 1 << 1,
	MAL_NORMAL = 1 << 2,
	MAL_TANGENT = 1 << 3,
	MAL_BLEND_WEIGHTS = 1 << 4,
	MAL_TEXCOORD0 = 1 << 5,
	MAL_TEXCOORD1 = 1 << 6,
};

enum MeshAttributeSemantic
{
	MAS_POSITION = 1,
	MAS_BLEND_WEIGHTS = 2,
	MAS_BLEND_INDICES = 3,
	MAS_NORMAL = 4,
	MAS_COLOR = 5,
	MAS_TEXCOORD = 6,
	MAS_BITANGENT = 7,
	MAS_TANGENT = 8,
};

enum MeshAttributeType
{
	MAT_FLOAT1 = 0,
	MAT_FLOAT2 = 1,
	MAT_FLOAT3 = 2,
	MAT_FLOAT4 = 3,
	MAT_COLOR = 4,
	MAT_SHORT1 = 5,
	MAT_SHORT2 = 6,
	MAT_SHORT4 = 8,
	MAT_UBYTE4 = 9,
	MAT_COLOR_ARGB = 10,
	MAT_COLOR_ABGR = 11,
	MAT_UINT4 = 12,
	MAT_INT4 = 13,
	MAT_USHORT1 = 14,
	MAT_USHORT2 = 15,
	MAT_USHORT4 = 17,
	MAT_INT1 = 18,
	MAT_INT2 = 19,
	MAT_INT3 = 20,
	MAT_UINT1 = 21,
	MAT_UINT2 = 22,
	MAT_UINT3 = 23,
	MAT_UBYTE4_NORM = 24,
	MAT_COUNT,
	VET_UNKNOWN = 0xffff
};

struct BoneWeight
{
    int index0;
    int index1;
    int index2;
    int index3;

    float weight0;
    float weight1;
    float weight2;
    float weight3;
};

class MeshAttribute
{
public:
	MeshAttribute(MeshAttributeType type, MeshAttributeSemantic semantic)
	{
		mType = type;
		mSemantic = semantic;
		mOffset = 0;
		mSize = getTypeSize(type);
	}
	~MeshAttribute()
	{
	}
	bool operator== (const MeshAttribute& rhs) const;
	bool operator!= (const MeshAttribute& rhs) const;

	uint32_t getOffset() const { return mOffset; }

	MeshAttributeType getType() const { return mType; }

	MeshAttributeSemantic getSemantic() const { return mSemantic; }

	uint32_t getSize() { return mSize; }
	// 获取数据类型的size
	static uint32_t getTypeSize(MeshAttributeType type);
	// 获取数据类型所包含基础数据类型的个数
	static uint16_t getTypeCount(MeshAttributeType type);
protected:
	uint32_t mOffset;
	uint32_t mSize;
	MeshAttributeType mType;
	MeshAttributeSemantic mSemantic;
};

class MeshDataDescription
{
public:
    MeshDataDescription();
    MeshDataDescription(const uint32_t& layout);
	~MeshDataDescription();
	void addMeshAttribute(MeshAttributeType type, MeshAttributeSemantic semantic);

	bool hasMeshAttribute(MeshAttributeSemantic semantic);

	uint32_t getMeshAttributeSize(MeshAttributeSemantic semantic);

	uint32_t getMeshAttributeOffset(MeshAttributeSemantic semantic);

	uint32_t getMeshAttributeStride();

private:
	void clearIfItExists(MeshAttributeType type, MeshAttributeSemantic semantic);
private:
	std::vector<MeshAttribute> mAttributes;
};

// data = indexbuf + vertexbuf
class MeshData
{
public:
	MeshData(uint32_t numVertices, uint32_t numIndices, MeshDataDescription* desc);
	~MeshData();
	MeshDataDescription* getMeshDesc() { return mDesc; }
	void setIndexes(void* data, uint32_t size);
	void setAttribute(MeshAttributeSemantic semantic, void* data, uint32_t size);
	uint8_t* getIndices();
	uint8_t* getVertices();
	void getAttribute(MeshAttributeSemantic semantic, void* data, uint32_t size);
	uint32_t getNumVertices() { return mNumVertices; }
	uint32_t getNumIndices() { return mNumIndices; }
    uint32_t getVertexBufferSize() const;
	uint32_t getIndexBufferSize() const;
	uint32_t getVertexStrideSize() const;
	uint32_t getStreamSize() const;
private:
	uint8_t* mData;
	MeshDataDescription* mDesc;
	uint32_t mNumVertices;
	uint32_t mNumIndices;
};

EFFECTS_NAMESPACE_END
