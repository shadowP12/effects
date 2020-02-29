#include "Scene.h"
#include "Component.h"
EFFECTS_NAMESPACE_BEGIN

SceneObject::SceneObject()
{
    SceneManager::instance().addSceneObject(std::dynamic_pointer_cast<SceneObject>(shared_from_this()));
}

SceneObject::~SceneObject()
{
}

void SceneObject::update()
{
    for (int i = 0; i < mComponents.size(); ++i)
    {
        mComponents[i]->update();
    }
}

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::addSceneObject(std::shared_ptr<SceneObject> obj)
{
    // todo: add check process
    mSceneObjects.push_back(obj);
}

void SceneManager::update()
{
    for (int i = 0; i < mSceneObjects.size(); ++i)
    {
        mSceneObjects[i]->update();
    }
}

EFFECTS_NAMESPACE_END