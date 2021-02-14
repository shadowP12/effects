#pragma once
#include "Core/Base.h"
#include "Scene/Components/Component.h"
#include "Math/Math.h"
EFFECTS_NAMESPACE_BEGIN

class SceneNode;
class SeaRenderable;
class Mesh;
class Material;

class CSea : public Component {
public:
    CSea(SceneNode* node);
    virtual ~CSea();
    static ComponentType getClassType() { return ComponentType::Sea; }
    ComponentType getType() override { return ComponentType::Sea; }
    void onNodeDirty() override;
    void initialized() override;
    void setMesh(std::shared_ptr<Mesh> mesh);
    void setMaterial(std::shared_ptr<Material> material);
    std::shared_ptr<Mesh> getMesh();
    std::shared_ptr<Material> getMaterial();
protected:
    friend SeaRenderable;
    SeaRenderable* mInternal;
};

EFFECTS_NAMESPACE_END
