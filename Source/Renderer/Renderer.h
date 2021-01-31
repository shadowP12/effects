#pragma once

#include "Core/Base.h"
#include "Core/Gfx/GfxResources.h"
#include "Core/Utility/Module.h"
#include <vector>
#include <memory>
EFFECTS_NAMESPACE_BEGIN

class Renderable;
class RenderView;
class GpuProgramDefines;
class Camera;
class Mesh;

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
    std::shared_ptr<Mesh> getQuadMesh() { return mQuadMesh; }
private:
    std::shared_ptr<Mesh> mQuadMesh;
    std::vector<Renderable*> mRenderables;
    std::vector<RenderView*> mViews;
};

EFFECTS_NAMESPACE_END