#pragma once
#include "../Base.h"
#include "../Data/MeshData.h"
#include "../Gfx/Gfx.h"
#include "Node.h"
#include <vector>
#include <string>
EFFECTS_NAMESPACE_BEGIN

class SceneNode : public Node
{
public:
	SceneNode();
	~SceneNode();
public:
	uint32_t mesh;
};

class Scene
{
public:
	Scene();
	~Scene();

private:
	std::vector<SceneNode> mSceneNodes;
};

EFFECTS_NAMESPACE_END