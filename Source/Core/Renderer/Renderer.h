#pragma once

#include "../Base.h"
#include "../Utility/Module.h"
#include "../Gfx/GpuProgram.h"
#include <vector>
EFFECTS_NAMESPACE_BEGIN

class Renderable;
class RenderView;
class GpuProgramDefines;
class Renderer : public Module<Renderer>
{
public:
    Renderer();
    ~Renderer();
    void render();
    void notifyRenderViewAdded(std::shared_ptr<RenderView> view);
    void notifyRenderableAdded(std::shared_ptr<Renderable> renderable);
private:
    std::vector<std::shared_ptr<Renderable>> mRenderables;
    std::shared_ptr<RenderView> mMainView;
    GpuProgramDefines mGlobalDefines;
};

EFFECTS_NAMESPACE_END