#include "Material.h"
EFFECTS_NAMESPACE_BEGIN

Material::Material()
{

}

Material::~Material()
{

}

void Material::setType(const MaterialType type)
{
    mType = type;
}

EFFECTS_NAMESPACE_END