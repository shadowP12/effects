#pragma once

#include "Core/Base.h"
#include "Core/Gfx/GfxResources.h"
#include "Math/Math.h"
#include "Renderer/Renderable.h"
#include <memory>
EFFECTS_NAMESPACE_BEGIN

class Mesh;
class Material;
class CSea;
class SeaRenderable : public Renderable {
public:
    SeaRenderable(CSea* csea);
    ~SeaRenderable();
    void initialize() override;
    void updateSeaMap();
    void setTransform(const glm::mat4& mat);
    void setMesh(std::shared_ptr<Mesh> mesh);
    void setMaterial(std::shared_ptr<Material> material);
    glm::mat4 getTransform();
    std::shared_ptr<Mesh> getMesh();
    std::shared_ptr<Material> getMaterial();
    GfxTexture* getHeightMap() { return mHeightMap; }
    GfxTexture* getNormalMap() { return mNormalMap; }
protected:
    friend class CSea;
    CSea* mCSea;
    glm::mat4 mWorldMatrix;
    std::shared_ptr<Mesh> mMesh;
    std::shared_ptr<Material> mMaterial;
    bool mShouldReset = true;
    GfxFramebuffer* mProcessFB = nullptr;
    GfxTexture* mHeightMap = nullptr;
    GfxTexture* mNormalMap = nullptr;
    GfxSampler* mSampler = nullptr;
    GfxProgram* mSeaOctave = nullptr;
    GfxProgram* mSeaNormal = nullptr;
};

EFFECTS_NAMESPACE_END