#include "Renderer/MeshRenderable.h"
#include "Renderer/Renderer.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
EFFECTS_NAMESPACE_BEGIN

MeshRenderable::MeshRenderable(){
    mType = RenderableType::MESH;
}

MeshRenderable::~MeshRenderable() {
}

void MeshRenderable::initialize() {
}

void MeshRenderable::setMesh(std::shared_ptr<Mesh> mesh) {
    mMesh = mesh;
}

void MeshRenderable::setMaterial(std::shared_ptr<Material> material) {
    mMaterial = material;
}

void MeshRenderable::setTransform(const glm::mat4 &mat) {
    mWorldMatrix = mat;
}

std::shared_ptr<Mesh> MeshRenderable::getMesh() {
    return mMesh;
}

std::shared_ptr<Material> MeshRenderable::getMaterial() {
    return mMaterial;
}

glm::mat4 MeshRenderable::getTransform() {
    return mWorldMatrix;
}

EFFECTS_NAMESPACE_END
