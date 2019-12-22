#include "Mesh.h"

EFFECTS_NAMESPACE_BEGIN

Mesh::Mesh(MeshData* meshData)
	:mMeshData(meshData)
{
	init();
}

Mesh::~Mesh()
{
	delete mMeshData;
}

void Mesh::init()
{
	glGenVertexArrays(1, &mVertexArray);
	glGenBuffers(1, &mVertexBuffer);
	glGenBuffers(1, &mIndexBuffer);

	glBindVertexArray(mVertexArray);

	uint8_t* vertexData = mMeshData->getVertices();
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMeshData->getStreamSize(), vertexData, GL_STATIC_DRAW);

	uint8_t* indexData = mMeshData->getIndices();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMeshData->getIndexBufferSize(), indexData, GL_STATIC_DRAW);

	/*
	//这里对Shader中的顶点输入做出规范:
	//1.Position     0
	//2.Normal       1
	//3.Tangent      2
	//4.UV0          3
	//5.Color        4
	//6.BlendIndices 5
	//7.BlendWeights 6
	*/
	if (mMeshData->getMeshDesc()->hasMeshAttribute(MAS_POSITION))
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mMeshData->getMeshDesc()->getMeshAttributeStride, (GLvoid*)mMeshData->getMeshDesc()->getMeshAttributeOffset(MAS_POSITION));
	}
	if (mMeshData->getMeshDesc()->hasMeshAttribute(MAS_NORMAL))
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mMeshData->getMeshDesc()->getMeshAttributeStride, (GLvoid*)mMeshData->getMeshDesc()->getMeshAttributeOffset(MAS_NORMAL));
	}
	if (mMeshData->getMeshDesc()->hasMeshAttribute(MAS_TANGENT))
	{
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, mMeshData->getMeshDesc()->getMeshAttributeStride, (GLvoid*)mMeshData->getMeshDesc()->getMeshAttributeOffset(MAS_TANGENT));
	}
	if (mMeshData->getMeshDesc()->hasMeshAttribute(MAS_TEXCOORD))
	{
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, mMeshData->getMeshDesc()->getMeshAttributeStride, (GLvoid*)mMeshData->getMeshDesc()->getMeshAttributeOffset(MAS_TEXCOORD));
	}
	if (mMeshData->getMeshDesc()->hasMeshAttribute(MAS_COLOR))
	{
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, mMeshData->getMeshDesc()->getMeshAttributeStride, (GLvoid*)mMeshData->getMeshDesc()->getMeshAttributeOffset(MAS_COLOR));
	}
	if (mMeshData->getMeshDesc()->hasMeshAttribute(MAS_BLEND_INDICES) | mMeshData->getMeshDesc()->hasMeshAttribute(MAS_BLEND_WEIGHTS))
	{
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_INT, GL_FALSE, mMeshData->getMeshDesc()->getMeshAttributeStride, (GLvoid*)mMeshData->getMeshDesc()->getMeshAttributeOffset(MAS_BLEND_INDICES));

		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, mMeshData->getMeshDesc()->getMeshAttributeStride, (GLvoid*)mMeshData->getMeshDesc()->getMeshAttributeOffset(MAS_BLEND_WEIGHTS));
	}

	glBindVertexArray(0);
}

void Mesh::draw()
{
	glBindVertexArray(mVertexArray);
	glDrawElements(GL_TRIANGLES, mMeshData->getNumIndices(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

MeshPool::MeshPool()
{
	mNextID = 0;
}

MeshPool::~MeshPool()
{
	for (auto& mesh : mMeshs)
	{
		delete mesh.second;
	}
}

uint32_t MeshPool::createMesh(MeshData* meshData)
{
	Mesh* mesh = new Mesh(meshData);
	uint32_t id = mNextID;
	mMeshs[id] = mesh;
	mNextID++;
	return id;
}

EFFECTS_NAMESPACE_END