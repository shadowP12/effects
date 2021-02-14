#pragma once

#include "Core/Base.h"
#include "Core/Gfx/GfxResources.h"
#include "Core/Utility/Module.h"
#include <vector>
#include <memory>
EFFECTS_NAMESPACE_BEGIN

class Renderable;
class RenderView;
class RenderLight;
class GpuProgramDefines;
class Camera;
class Mesh;
class DefaultEffect;
class SeaEffect;

class Renderer : public Module<Renderer>
{
public:
    Renderer();
    ~Renderer();
    void render();
    RenderView* addRenderView();
    void addRenderable(Renderable* renderable);
    void addRenderLight(RenderLight* renderLight);
    std::vector<RenderView*>& getViews() { return mViews; }
    std::vector<Renderable*>& getRenderables() { return mRenderables; }
    std::shared_ptr<Mesh> getQuadMesh() { return mQuadMesh; }
    const std::vector<RenderLight*>& getDirectLights() { return mDirectLights; }
    const std::vector<RenderLight*>& getPointLights() { return mPointLights; }
    void setLightDirty() { mLightDirty = true; }
private:
    std::shared_ptr<Mesh> mQuadMesh;
    std::vector<Renderable*> mRenderables;
    std::vector<RenderView*> mViews;
    std::vector<RenderLight*> mLights;
    std::vector<RenderLight*> mDirectLights;
    std::vector<RenderLight*> mPointLights;
    std::shared_ptr<DefaultEffect> mDefaultEffect;
    std::shared_ptr<SeaEffect> mSeaEffect;
    bool mLightDirty = true;
};

EFFECTS_NAMESPACE_END