#pragma once
#include <vector>
#include <map>
#include <memory>
#include "Core/Base.h"
#include "Core/Utility/Module.h"
#include "Node.h"
EFFECTS_NAMESPACE_BEGIN
class MeshData;
class Component;
class SceneManager;
class SceneObject : public Node
{
public:
    SceneObject();
    ~SceneObject();
    template<class T, class... Args>
    std::shared_ptr<T> addComponent(Args &&... args)
    {
        std::shared_ptr<T> Component = std::make_shared<T>(std::dynamic_pointer_cast<SceneObject>(shared_from_this()), std::forward<Args>(args)...);
        mComponents.push_back(Component);
        return Component;
    }
    template<class T>
    std::shared_ptr<T> getComponent()
    {
        for (int i = 0; i < mComponents.size(); ++i)
        {
            if(mComponents[i] T::getType() == T::getClassType())
            {
                return std::dynamic_pointer_cast<T>(mComponents[i]);
            }
        }

        return nullptr;
    }
    void initialized();
    void update();
protected:
    std::vector<std::shared_ptr<Component>> mComponents;
};

class SceneManager : public Module<SceneManager>
{
public:
    SceneManager();
    ~SceneManager();
    void update();
    void addSceneObject(std::shared_ptr<SceneObject> obj);
private:
    std::vector<std::shared_ptr<SceneObject>> mSceneObjects;
    std::shared_ptr<SceneObject> mRoot;
};

EFFECTS_NAMESPACE_END
