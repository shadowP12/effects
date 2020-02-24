#include "Scene.h"

EFFECTS_NAMESPACE_BEGIN

SceneObject::SceneObject()
{

}

SceneObject::~SceneObject()
{

}

Scene::Scene()
{

}

Scene::~Scene()
{
}

void Scene::addSceneObject(std::shared_ptr<SceneObject> obj)
{
    // todo: add check process
    mSceneObjects.push_back(obj);
}

EFFECTS_NAMESPACE_END