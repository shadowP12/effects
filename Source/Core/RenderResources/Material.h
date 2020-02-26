#pragma once

#include "../Base.h"
#include "../../Math/Math.h"
#include <string>
#include <unordered_map>

EFFECTS_NAMESPACE_BEGIN

enum MaterialType
{
    MT_PBR = 0,
};

class GpuProgramDefines;
class GpuProgram;

class Material
{
public:
    Material();
    ~Material();
    void setType(const MaterialType& type);
    void setProgram(GpuProgram* program);
    GpuProgram* getProgram();
    virtual void setProgramDefines(GpuProgramDefines& defines);
protected:
    MaterialType mType;
    GpuProgram* mProgram;
    std::unordered_map<std::string, int> mIntParams;
    std::unordered_map<std::string, float> mFloatParams;
    std::unordered_map<std::string, glm::vec3> mFloat3Params;
    std::unordered_map<std::string, glm::vec4> mFloat4Params;
};

EFFECTS_NAMESPACE_END