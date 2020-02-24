#pragma once

#include "../Base.h"
#include "../Utility/Module.h"
#include <vector>
EFFECTS_NAMESPACE_BEGIN

class Renderable;

class Renderer : public Module<Renderer>
{
public:
    Renderer();
    ~Renderer();
    void notifyRenderableAdded(std::shared_ptr<Renderable> renderable);
private:
    std::vector<std::shared_ptr<Renderable>> mRenderables;
};

EFFECTS_NAMESPACE_END