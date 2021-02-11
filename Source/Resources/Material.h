#pragma once

#include "Core/Base.h"
#include "Renderer/Effects/BaseEffect.h"
#include "Math/Math.h"
#include "Resources/Resource.h"
#include <string>
#include <unordered_map>

EFFECTS_NAMESPACE_BEGIN

class Material : public Resource
{
public:
    Material();
    ~Material();
    static std::shared_ptr<Material> create(const std::string& id);
    static void remove(std::shared_ptr<Material> material);

    void setEffectType(const EffectType& type) {
        mEffectType = type;
    }
    EffectType getEffectType() { return mEffectType; }
    std::unordered_map<std::string, int>& getIntParams(){return mIntParams;}
    std::unordered_map<std::string, float>& getFloatParams(){return mFloatParams;}
    std::unordered_map<std::string, glm::vec3>& getFloat3Params(){return mFloat3Params;}
    std::unordered_map<std::string, glm::vec4>& getFloat4Params(){return mFloat4Params;}
    std::unordered_map<std::string, std::string>& getStringParams(){return mStringParams;}
protected:
    EffectType mEffectType;
    std::unordered_map<std::string, int> mIntParams;
    std::unordered_map<std::string, float> mFloatParams;
    std::unordered_map<std::string, glm::vec3> mFloat3Params;
    std::unordered_map<std::string, glm::vec4> mFloat4Params;
    std::unordered_map<std::string, std::string> mStringParams;
};

EFFECTS_NAMESPACE_END