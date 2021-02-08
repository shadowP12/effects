#pragma once
#include <vector>
#include <map>
#include <memory>
#include "Core/Base.h"
#include "Core/Utility/Module.h"
#include "Scene/Components/Component.h"
#include "Math/Math.h"
EFFECTS_NAMESPACE_BEGIN
class MeshData;
class Component;
class SceneManager;

class SceneNode
{
public:
    static SceneNode* create();
    static void destroy(SceneNode* node);

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

    SceneNode* getParent() {
        return mParent;
    }

    std::vector<SceneNode*>& getChildren() {
        return mChildren;
    }
    void setParent(SceneNode* newParent);
    void appendChildren(SceneNode* newChildren);
    void removeChildren(SceneNode* oldChildren);
    void setTransform(const glm::vec3& pos, const glm::vec3& scale, const glm::quat& rot);
    glm::mat4 getLocalMatrix();
    glm::mat4 getWorldMatrix();
    void setPosition(const glm::vec3& pos);
    glm::vec3 getPosition(){ return mLPos; }
    void setRotation(const glm::quat& rot);
    glm::quat getRotation(){ return mLRot; }
    void setScale(const glm::vec3& scale);
    glm::vec3 getScale(){ return mLScale; }
    glm::vec3 getRightVector();
    glm::vec3 getUpVector();
    glm::vec3 getFrontVector();
    void rotate(const glm::vec3 axis, const float& angle);

private:
    SceneNode();
    ~SceneNode();
    void destroyInternal();
protected:
    friend class Scene;
    SceneNode* mParent;
    std::vector<SceneNode*> mChildren;
    std::vector<Component*> mComponents;
    glm::vec3 mLPos;
    glm::vec3 mLScale;
    glm::quat mLRot;
    glm::mat4 mLocalMatrix;
    glm::mat4 mWorldMatrix;
    bool mDirtyFlag;
};

class Scene : public Module<Scene>
{
public:
    Scene();
    ~Scene();
    void update(float dt);
    std::vector<SceneNode*>& getNodes() { return mNodes; }
    SceneNode* addNode();
    void removeNode(SceneNode* node);
private:
    SceneNode* mRoot = nullptr;
    std::vector<SceneNode*> mNodes;
};

EFFECTS_NAMESPACE_END
