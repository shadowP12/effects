#pragma once
#include "Core/Base.h"
#include "Scene/Components/Component.h"
#include "Math/Math.h"
EFFECTS_NAMESPACE_BEGIN

class SceneNode;
class Renderable;
class Mesh;
class Material;

class CRenderable : public Component {
public:
    CRenderable(SceneNode* node);
    virtual ~CRenderable();
    static ComponentType getClassType() { return ComponentType::Renderable; }
    ComponentType getType() override { return ComponentType::Renderable; }
    void onNodeDirty() override;
    void initialized() override;
    void setMesh(std::shared_ptr<Mesh> mesh);
    void setMaterial(std::shared_ptr<Material> material);
    std::shared_ptr<Mesh> getMesh();
    std::shared_ptr<Material> getMaterial();
protected:
    Renderable* mInternal;
};

EFFECTS_NAMESPACE_END
