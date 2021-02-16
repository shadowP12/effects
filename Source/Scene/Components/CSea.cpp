#include "Scene/Components/CSea.h"
#include "Scene/Scene.h"
#include "Renderer/Renderer.h"
#include "Renderer/SeaRenderable.h"
EFFECTS_NAMESPACE_BEGIN

CSea::CSea(SceneNode* node)
    :Component(node) {
    mInternal = new SeaRenderable(this);
    mInternal->mShouldReset = true;
    Renderer::instance().addRenderable(mInternal);
    mSeaX = 1024;
    mSeaY = 1024;
}

CSea::~CSea() {
}

void CSea::initialized() {
}

void CSea::onNodeDirty() {
    mInternal->setTransform(mNode->getWorldMatrix());
}

void CSea::update(float dt) {
    mSeaTime += dt;
}

void CSea::setMesh(std::shared_ptr<Mesh> mesh) {
    mInternal->setMesh(mesh);
}

void CSea::setMaterial(std::shared_ptr<Material> material) {
    mInternal->setMaterial(material);
}

std::shared_ptr<Mesh> CSea::getMesh() {
    return mInternal->getMesh();
}

std::shared_ptr<Material> CSea::getMaterial() {
    return mInternal->getMaterial();
}

EFFECTS_NAMESPACE_END