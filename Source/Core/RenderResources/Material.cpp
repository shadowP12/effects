#include "Material.h"
#include "../Gfx/GpuProgram.h"
EFFECTS_NAMESPACE_BEGIN

Material::Material()
{

}

Material::~Material()
{

}

void Material::setType(const MaterialType& type)
{
    mType = type;
}

void Material::setProgramDefines(GpuProgramDefines& defines)
{
}

GpuProgram* Material::getProgram()
{
    return mProgram;
}

void Material::setProgram(GpuProgram *program)
{
    mProgram = program;
}

EFFECTS_NAMESPACE_END