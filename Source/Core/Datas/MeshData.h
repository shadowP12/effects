#pragma once
#include "../Base.h"
#include "../Utility/Flags.h"
#include <vector>
EFFECTS_NAMESPACE_BEGIN

enum VertexSemantic
{
    SEMANTIC_UNDEFINED  = 1 << 0,
    SEMANTIC_POSITION   = 1 << 1,
    SEMANTIC_NORMAL     = 1 << 2,
    SEMANTIC_COLOR      = 1 << 3,
    SEMANTIC_TANGENT    = 1 << 4,
    SEMANTIC_BITANGENT  = 1 << 5,
    SEMANTIC_JOINTS     = 1 << 6,
    SEMANTIC_WEIGHTS    = 1 << 7,
    SEMANTIC_TEXCOORD0  = 1 << 8,
    SEMANTIC_TEXCOORD1  = 1 << 9,
    SEMANTIC_TEXCOORD2  = 1 << 10,
    SEMANTIC_TEXCOORD3  = 1 << 11,
    SEMANTIC_TEXCOORD4  = 1 << 12,
    SEMANTIC_TEXCOORD5  = 1 << 13,
    SEMANTIC_TEXCOORD6  = 1 << 14,
    SEMANTIC_TEXCOORD7  = 1 << 15,
    SEMANTIC_TEXCOORD8  = 1 << 16,
};
MAKE_ENUM_FLAG(uint32_t, VertexSemantic)

enum VertexType
{
    VERTEX_TYPE_FLOAT1 = 0,
    VERTEX_TYPE_FLOAT2,
	VERTEX_TYPE_FLOAT3,
	VERTEX_TYPE_FLOAT4,
	VERTEX_TYPE_COLOR,
	VERTEX_TYPE_SHORT1,
	VERTEX_TYPE_SHORT2,
	VERTEX_TYPE_SHORT4,
	VERTEX_TYPE_UBYTE4,
	VERTEX_TYPE_COLOR_ARGB,
	VERTEX_TYPE_COLOR_ABGR,
	VERTEX_TYPE_UINT4,
	VERTEX_TYPE_INT4,
	VERTEX_TYPE_USHORT1,
	VERTEX_TYPE_USHORT2,
	VERTEX_TYPE_USHORT4,
	VERTEX_TYPE_INT1,
	VERTEX_TYPE_INT2,
	VERTEX_TYPE_INT3,
	VERTEX_TYPE_UINT1,
	VERTEX_TYPE_UINT2,
	VERTEX_TYPE_UINT3,
	VERTEX_TYPE_UBYTE4_NORM,
	VERTEX_TYPE_COUNT,
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

class VertexAttribute
{
public:
    VertexAttribute(VertexType type, VertexSemantic semantic)
	{
		mType = type;
		mSemantic = semantic;
		mOffset = 0;
		mSize = getTypeSize(type);
	}
	~VertexAttribute()
	{
	}
	bool operator== (const VertexAttribute& rhs) const;
	bool operator!= (const VertexAttribute& rhs) const;

	uint32_t getOffset() const { return mOffset; }

    VertexType getType() const { return mType; }

    VertexSemantic getSemantic() const { return mSemantic; }

	uint32_t getSize() { return mSize; }
	// 获取数据类型的size
	static uint32_t getTypeSize(VertexType type);
	// 获取数据类型所包含基础数据类型的个数
	static uint16_t getTypeCount(VertexType type);
protected:
	uint32_t mOffset;
	uint32_t mSize;
    VertexType mType;
    VertexSemantic mSemantic;
};

class VertexLayout
{
public:
    VertexLayout();
    VertexLayout(const uint32_t& layout);
	~VertexLayout();
	void addVertexAttribute(VertexType type, VertexSemantic semantic);

	bool hasVertexAttribute(VertexSemantic semantic);

	uint32_t getVertexAttributeSize(VertexSemantic semantic);

	uint32_t getVertexAttributeOffset(VertexSemantic semantic);

	uint32_t getVertexAttributeStride();

	uint32_t getLayout(){return mLayout;}

private:
	void clearIfItExists(VertexType type, VertexSemantic semantic);
private:
	std::vector<VertexAttribute> mAttributes;
	uint32_t mLayout;
};

// data = indexbuf + vertexbuf
class MeshData
{
public:
	MeshData(uint32_t numVertices, uint32_t numIndices, VertexLayout* vertexLayout);
	~MeshData();
    VertexLayout* getVertexLayout() { return mVertexLayout; }
	void setIndexes(void* data, uint32_t size);
	void setAttribute(VertexSemantic semantic, void* data, uint32_t size);
    void getAttribute(VertexSemantic semantic, void* data, uint32_t size);
	uint8_t* getIndices();
	uint8_t* getVertices();
	uint32_t getNumVertices() { return mNumVertices; }
	uint32_t getNumIndices() { return mNumIndices; }
    uint32_t getVertexBufferSize() const;
	uint32_t getIndexBufferSize() const;
	uint32_t getVertexStrideSize() const;
	uint32_t getStreamSize() const;
private:
	uint8_t* mData;
    VertexLayout* mVertexLayout;
	uint32_t mNumVertices;
	uint32_t mNumIndices;
};

EFFECTS_NAMESPACE_END
