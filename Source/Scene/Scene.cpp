#include "Scene.h"
#include "Components/Component.h"
EFFECTS_NAMESPACE_BEGIN

SceneNode::SceneNode() {
}

SceneNode::~SceneNode() {
    for (int i = 0; i < mComponents.size(); ++i) {
        SAFE_DELETE(mComponents[i]);
    }
    mComponents.clear();
}

void SceneNode::initialized() {
}

void SceneNode::update(float dt) {
    for (int i = 0; i < mComponents.size(); ++i) {
        if (mDirtyFlag) {
            mComponents[i]->onNodeDirty();
        }
        mComponents[i]->update(dt);
    }
    if (mDirtyFlag) {
        mDirtyFlag = false;
    }
}

Scene::Scene(){
    mRootNode = new SceneNode();
    mNodes.push_back(mRootNode);
}

Scene::~Scene(){
    SAFE_DELETE(mRootNode);
    mNodes.clear();
}

SceneNode* Scene::addNode() {
    SceneNode* node = new SceneNode();
    node->setParent(mRootNode);
    mNodes.push_back(node);
    return node;
}

void Scene::update(float dt) {
    for (int i = 0; i < mNodes.size(); ++i) {
        mNodes[i]->update(dt);
    }
}

EFFECTS_NAMESPACE_END