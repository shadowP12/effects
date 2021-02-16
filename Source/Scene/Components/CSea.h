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
    void update(float dt) override;
    void setMesh(std::shared_ptr<Mesh> mesh);
    void setMaterial(std::shared_ptr<Material> material);
    std::shared_ptr<Mesh> getMesh();
    std::shared_ptr<Material> getMaterial();
protected:
    friend SeaRenderable;
    SeaRenderable* mInternal;
    int mSeaX = 1;
    int mSeaY = 1;
    float mSeaTime = 0.0f;
    const float mSeaHeight = 0.6f;
    const float mSeaChoppy = 4.0f;
    const float mSeaSpeed = 0.8f;
    const float mSeaFreq = 0.16f;
};

EFFECTS_NAMESPACE_END
