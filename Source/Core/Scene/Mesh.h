#pragma once
#include "../Base.h"
#include "../Data/MeshData.h"
#include "../Gfx/Gfx.h"
#include "../Utility/Module.h"
#include <map>
#include <vector>
#include <string>
EFFECTS_NAMESPACE_BEGIN

class Mesh
{
public:
	Mesh(MeshData* meshData);
	~Mesh();
	void draw();
private:
	void init();
private:
	MeshData* mMeshData;
	GLuint mVertexBuffer;
	GLuint mIndexBuffer;
	GLuint mVertexArray;
};

class MeshPool : public Module<MeshPool>
{
public:
	MeshPool();
	~MeshPool();
	uint32_t createMesh(MeshData* meshData);
private:
	static uint32_t mNextID;
	std::map<uint32_t, Mesh*> mMeshs;
};
EFFECTS_NAMESPACE_END