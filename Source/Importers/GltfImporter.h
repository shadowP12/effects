#pragma once
#include "../Core/Base.h"
#include "../Core/Utility/Flags.h"
#include "../Math/Math.h"
#include <string>
#include <map>
#include <memory>
#include <vector>

EFFECTS_NAMESPACE_BEGIN

class Scene;
class Mesh;
class Node;
class GfxTexture;
struct GfxSampler;

enum PBRMaterialBit
{
    PBR_UNDEFINED               = 1 << 0,
    PBR_ALPHA                   = 1 << 1,
    PBR_BASE_COLOR_MAP          = 1 << 2,
    PBR_NORMAL_MAP              = 1 << 3,
    PBR_METALLIC_ROUGHNESS_MAP  = 1 << 4,
};
MAKE_ENUM_FLAG(uint32_t, PBRMaterialBit)

enum GltfAlphaMode
{
    ALPHA_MODE_OPAQUE,
    ALPHA_MODE_MASK,
    ALPHA_MODE_BLEND,
};

struct GltfTextureView
{
    GfxTexture* texture;
    GfxSampler* sampler;
};

struct GltfMaterial
{
    GltfTextureView baseColorMap;
    GltfTextureView normalMap;
    GltfTextureView metallicRoughnessMap;
    glm::vec4 baseColor;
    float metallic;
    float roughness;
    bool doubleSided;
    GltfAlphaMode alphaMode;
    PBRMaterialBit bits;
};

struct GltfScene
{
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<Mesh*> meshs;
    std::vector<GfxTexture*> textures;
    std::vector<GfxSampler*> samples;
    std::vector<GltfMaterial*> materials;
    std::map<std::shared_ptr<Node>, Mesh*> meshHelper;
    std::map<Mesh*, GltfMaterial*> materialHelper;
};

void destroyGltfScene(GltfScene* scene);

class GltfImporter
{
public:
    GltfImporter();
    ~GltfImporter();
    void load(std::string filePath, GltfScene* scene);
private:
};

EFFECTS_NAMESPACE_END