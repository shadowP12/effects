#pragma once

#include "Core/Base.h"
#include "Renderer/Effects/BaseEffect.h"
#include "Math/Math.h"
#include "Resources/Resource.h"
#include <string>
#include <unordered_map>

EFFECTS_NAMESPACE_BEGIN
class Texture;

typedef int MaterialInt;

typedef float MaterialFloat;

typedef bool MaterialBool;

typedef glm::vec3 MaterialFloat3;

typedef glm::vec4 MaterialFloat4;

typedef std::string MaterialString;

typedef std::shared_ptr<Texture> MaterialTexture;

struct MaterialCombo {
    int idx;
    std::vector<std::string> fields;
};

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
    std::unordered_map<std::string, MaterialInt>& getIntParams() { return mIntParams; }
    std::unordered_map<std::string, MaterialBool>& getBoolParams() { return mBoolParams; }
    std::unordered_map<std::string, MaterialFloat>& getFloatParams() { return mFloatParams; }
    std::unordered_map<std::string, MaterialFloat3>& getFloat3Params() { return mFloat3Params; }
    std::unordered_map<std::string, MaterialFloat4>& getFloat4Params() { return mFloat4Params; }
    std::unordered_map<std::string, MaterialString>& getStringParams() { return mStringParams; }
    std::unordered_map<std::string, MaterialCombo>& getComboParams() { return mComboParams; }
    std::unordered_map<std::string, MaterialTexture>& getTextureParams() { return mTextureParams; }

protected:
    EffectType mEffectType;
    std::unordered_map<std::string, MaterialInt> mIntParams;
    std::unordered_map<std::string, MaterialBool> mBoolParams;
    std::unordered_map<std::string, MaterialFloat> mFloatParams;
    std::unordered_map<std::string, MaterialFloat3> mFloat3Params;
    std::unordered_map<std::string, MaterialFloat4> mFloat4Params;
    std::unordered_map<std::string, MaterialString> mStringParams;
    std::unordered_map<std::string, MaterialCombo> mComboParams;
    std::unordered_map<std::string, MaterialTexture> mTextureParams;
};

EFFECTS_NAMESPACE_END