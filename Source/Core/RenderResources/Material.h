#pragma once

#include "../Base.h"
#include "../../Math/Math.h"
#include <string>
#include <unordered_map>

EFFECTS_NAMESPACE_BEGIN

enum class MaterialType
{
    PBR = 0,
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
    std::unordered_map<std::string, int>& getIntParams(){return mIntParams;}
    std::unordered_map<std::string, float>& getFloatParams(){return mFloatParams;}
    std::unordered_map<std::string, glm::vec3>& getFloat3Params(){return mFloat3Params;}
    std::unordered_map<std::string, glm::vec4>& getFloat4Params(){return mFloat4Params;}
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