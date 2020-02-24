#pragma once

#include "../Base.h"

EFFECTS_NAMESPACE_BEGIN

enum MaterialType
{
    MT_PBR = 0,
};

class Material
{
public:
    Material();
    ~Material();
    void setType(const MaterialType type);
protected:
    MaterialType mType;
};

EFFECTS_NAMESPACE_END