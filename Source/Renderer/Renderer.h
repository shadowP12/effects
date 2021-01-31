#pragma once

#include "Core/Base.h"
#include "Core/Utility/Module.h"
#include <vector>
EFFECTS_NAMESPACE_BEGIN

class Renderable;
class RenderView;
class GpuProgramDefines;
class Camera;
class Renderer : public Module<Renderer>
{
public:
    Renderer();
    ~Renderer();
    void render();
    RenderView* addRenderView();
    Renderable* addRenderable();
    std::vector<RenderView*>& getViews() { return mViews; }
    std::vector<Renderable*>& getRenderables() { return mRenderables; }
private:
    std::vector<Renderable*> mRenderables;
    std::vector<RenderView*> mViews;


};

EFFECTS_NAMESPACE_END