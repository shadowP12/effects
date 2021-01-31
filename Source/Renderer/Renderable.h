#pragma once

#include "Core/Base.h"
#include "Math/Math.h"
#include <memory>
EFFECTS_NAMESPACE_BEGIN

enum class RenderableAnimType
{
    None,
    Skinned,
};

class Mesh;
class Material;

class Renderable
{
public:
    Renderable();
    ~Renderable();
    void initialize();
    void setTransform(const glm::mat4& mat);
    void setMesh(std::shared_ptr<Mesh> mesh);
    void setMaterial(std::shared_ptr<Material> material);
    glm::mat4 getTransform();
    std::shared_ptr<Mesh> getMesh();
    std::shared_ptr<Material> getMaterial();

protected:
    glm::mat4 mWorldMatrix;
    std::shared_ptr<Mesh> mMesh;
    std::shared_ptr<Material> mMaterial;
};

EFFECTS_NAMESPACE_END