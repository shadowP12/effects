#include "Resources/Material.h"
#include "Resources/ResourceManager.h"
#include "Core/Utility/UUID.h"
EFFECTS_NAMESPACE_BEGIN

Material::Material()
{
}

Material::~Material()
{
}

std::shared_ptr<Material> Material::create(const std::string& id) {
    std::shared_ptr<Material> material = std::make_shared<Material>();
    material->mId = id;
    material->mUUID = generateUUID();
    material->mType = ResourceType::MATERIAL;
    ResourceManager::instance().addRes(material);
    return material;
}

void Material::remove(std::shared_ptr<Material> material) {
    ResourceManager::instance().removeRes(material);
}

EFFECTS_NAMESPACE_END