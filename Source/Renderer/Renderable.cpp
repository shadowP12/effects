#include "Renderable.h"
#include "Renderer.h"
#include "../RenderResources/Mesh.h"
#include "../RenderResources/Material.h"
EFFECTS_NAMESPACE_BEGIN

Renderable::Renderable()
{

}

Renderable::~Renderable() {
}

void Renderable::initialize() {
}

void Renderable::setMesh(std::shared_ptr<Mesh> mesh)
{
    mMesh = mesh;
}

void Renderable::setMaterial(std::shared_ptr<Material> material)
{
    mMaterial = material;
}

void Renderable::setTransform(const glm::mat4 &mat)
{
    mWorldMatrix = mat;
}

std::shared_ptr<Mesh> Renderable::getMesh()
{
    return mMesh;
}

std::shared_ptr<Material> Renderable::getMaterial()
{
    return mMaterial;
}

glm::mat4 Renderable::getTransform()
{
    return mWorldMatrix;
}

EFFECTS_NAMESPACE_END
