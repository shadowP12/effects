#pragma once
#include <vector>
#include <map>
#include <memory>
#include "Core/Base.h"
#include "Core/Utility/Module.h"
#include "Scene/Components/Component.h"
#include "Node.h"
EFFECTS_NAMESPACE_BEGIN
class MeshData;
class Component;
class SceneManager;

class SceneNode : public Node
{
public:
    SceneNode();
    ~SceneNode();
    template<class T, class... Args>
    T* addComponent(Args &&... args) {
        T* component = new T(this, std::forward<Args>(args)...);
        mComponents.push_back(component);
        return component;
    }

    template<class T>
    T* getComponent() {
        for (int i = 0; i < mComponents.size(); ++i) {
            if(mComponents[i]->getType() == T::getClassType()) {
                return (T*)mComponents[i];
            }
        }
        return nullptr;
    }
    void initialized();
    void update(float dt);
protected:
    std::vector<Component*> mComponents;
};

class Scene : public Module<Scene>
{
public:
    Scene();
    ~Scene();
    void update(float dt);
    SceneNode* addNode();
private:
    std::vector<SceneNode*> mNodes;
    SceneNode* mRootNode = nullptr;
};

EFFECTS_NAMESPACE_END
