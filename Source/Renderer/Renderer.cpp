#include "Renderer.h"
#include "Renderable.h"
#include "RenderView.h"
#include "RenderResources/Mesh.h"
#include "RenderResources/Material.h"
#include "Renderer/Effects/DefaultEffect.h"
EFFECTS_NAMESPACE_BEGIN

Renderer::Renderer() {
    mQuadMesh = std::shared_ptr<Mesh>(genQuadMesh());
    mDefaultEffect = std::make_shared<DefaultEffect>();
}

Renderer::~Renderer() {
    for (int i = 0; i < mViews.size(); ++i) {
        SAFE_DELETE(mViews[i]);
    }
    mViews.clear();

    for (int i = 0; i < mRenderables.size(); ++i) {
        SAFE_DELETE(mRenderables[i]);
    }
    mRenderables.clear();
}

RenderView* Renderer::addRenderView() {
    RenderView* view = new RenderView();
    mViews.push_back(view);
    return view;
}

Renderable* Renderer::addRenderable() {
    Renderable* renderable = new Renderable();
    mRenderables.push_back(renderable);
    return renderable;
}

void Renderer::render()
{
    mDefaultEffect->render(mViews, mRenderables);
}

EFFECTS_NAMESPACE_END