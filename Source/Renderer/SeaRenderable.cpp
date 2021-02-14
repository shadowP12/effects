#include "Renderer/SeaRenderable.h"
#include "Renderer/Renderer.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
EFFECTS_NAMESPACE_BEGIN

SeaRenderable::SeaRenderable(CSea* csea){
    mCSea = csea;
    mType = RenderableType::SEA;
}

SeaRenderable::~SeaRenderable() {
}

void SeaRenderable::initialize() {
}

void SeaRenderable::setMesh(std::shared_ptr<Mesh> mesh) {
    mMesh = mesh;
}

void SeaRenderable::setMaterial(std::shared_ptr<Material> material) {
    mMaterial = material;
}

void SeaRenderable::setTransform(const glm::mat4 &mat) {
    mWorldMatrix = mat;
}

std::shared_ptr<Mesh> SeaRenderable::getMesh() {
    return mMesh;
}

std::shared_ptr<Material> SeaRenderable::getMaterial() {
    return mMaterial;
}

glm::mat4 SeaRenderable::getTransform() {
    return mWorldMatrix;
}

void SeaRenderable::updateSeaMap() {
    if (mShouldReset) {

        mShouldReset = false;
    }

}

EFFECTS_NAMESPACE_END
