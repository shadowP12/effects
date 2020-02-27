#pragma once

#include "../Base.h"
#include "../../Math/Math.h"
#include "Material.h"
#include <string>

EFFECTS_NAMESPACE_BEGIN

class PBRMaterial : public Material
{
public:
    PBRMaterial();
    ~PBRMaterial();
    virtual void setProgramDefines(GpuProgramDefines& defines) override;
};

EFFECTS_NAMESPACE_END
