#pragma once
#include <vector>
#include <map>
#include <memory>
#include "../Base.h"
#include "Node.h"
EFFECTS_NAMESPACE_BEGIN
class MeshData;
class Component;

class SceneObject : public Node
{
public:
    SceneObject();
    ~SceneObject();
};

class Scene
{
public:
    Scene();
    ~Scene();
    void addSceneObject(std::shared_ptr<SceneObject> obj);
private:
    // meshs
    // materials
    std::vector<std::shared_ptr<SceneObject>> mSceneObjects;
    std::shared_ptr<SceneObject> mRoot;
};

EFFECTS_NAMESPACE_END
