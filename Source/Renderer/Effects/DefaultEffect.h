#pragma once
#include "Core/Base.h"
#include "BaseEffect.h"
#include <map>
EFFECTS_NAMESPACE_BEGIN

class DefaultEffect : public BaseEffect {
public:
    DefaultEffect();
    ~DefaultEffect();
    void render(std::vector<RenderView*>, std::vector<Renderable*>) override;
private:
    GfxProgram* getProgram(Renderable* renderable);

private:
    std::map<size_t, GfxProgram*> mProgramCache;
};

EFFECTS_NAMESPACE_END
