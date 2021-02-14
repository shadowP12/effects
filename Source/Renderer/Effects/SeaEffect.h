#pragma once
#include "Core/Base.h"
#include "Core/Utility/Flags.h"
#include "BaseEffect.h"
#include <map>
EFFECTS_NAMESPACE_BEGIN
class SeaRenderable;

class SeaEffect : public BaseEffect {
public:
    enum class Bit {
        USE_DIRECT_LIGHT = 1 << 0,
    };
    SeaEffect();
    ~SeaEffect();
    void render(std::vector<RenderView*>, std::vector<Renderable*>) override;
private:
    GfxProgram* getProgram(uint32_t bits);
private:
    std::map<uint32_t, GfxProgram*> mProgramCache;
};

EFFECTS_NAMESPACE_END
