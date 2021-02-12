#include "Scene/Components/CRenderable.h"
#include "Scene/Scene.h"
#include "Renderer/Renderable.h"
#include "Renderer/Renderer.h"
EFFECTS_NAMESPACE_BEGIN

CRenderable::CRenderable(SceneNode* node)
    :Component(node) {
    mInternal = Renderer::instance().addRenderable();
}

CRenderable::~CRenderable() {
}

void CRenderable::initialized() {
    mInternal->initialize();
}

void CRenderable::onNodeDirty() {
    mInternal->setTransform(mNode->getLocalMatrix());
}

void CRenderable::setMesh(std::shared_ptr<Mesh> mesh) {
    mInternal->setMesh(mesh);
}

void CRenderable::setMaterial(std::shared_ptr<Material> material) {
    mInternal->setMaterial(material);
}

std::shared_ptr<Mesh> CRenderable::getMesh() {
    return mInternal->getMesh();
}

std::shared_ptr<Material> CRenderable::getMaterial() {
    return mInternal->getMaterial();
}

EFFECTS_NAMESPACE_END