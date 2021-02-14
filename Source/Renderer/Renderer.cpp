#include "Renderer.h"
#include "Renderable.h"
#include "RenderView.h"
#include "RenderLight.h"
#include "Renderer/MeshRenderable.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "Renderer/Effects/DefaultEffect.h"
#include "Renderer/Effects/SeaEffect.h"
EFFECTS_NAMESPACE_BEGIN

Renderer::Renderer() {
    mQuadMesh = genQuadMesh();
    mDefaultEffect = std::make_shared<DefaultEffect>();
    mSeaEffect = std::make_shared<SeaEffect>();
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

void Renderer::addRenderable(Renderable* renderable) {
    mRenderables.push_back(renderable);
}

void Renderer::addRenderLight(RenderLight *renderLight) {
    mLightDirty = true;
    mLights.push_back(renderLight);
}

void Renderer::render()
{
    if (mLightDirty) {
        mDirectLights.clear();
        mPointLights.clear();
        for (int i = 0; i < mLights.size(); ++i) {
            if (mLights[i]->getType() == RenderLightType::DIRECT_LIGHT) {
                mDirectLights.push_back(mLights[i]);
            } else if (mLights[i]->getType() == RenderLightType::POINT_LIGHT) {
                mPointLights.push_back(mLights[i]);
            }
        }
        mLightDirty = false;
    }
    std::vector<Renderable*> defaultRenderables;
    std::vector<Renderable*> seaRenderables;
    for (int i = 0; i < mRenderables.size(); ++i) {
        if (mRenderables[i]->getType() == RenderableType::MESH) {
            MeshRenderable* meshRenderable = (MeshRenderable*)mRenderables[i];
            if (meshRenderable->getMaterial()->getEffectType() == EffectType::DEFAULT) {
                defaultRenderables.push_back(mRenderables[i]);
            }
        } else if (mRenderables[i]->getType() == RenderableType::SEA) {
            seaRenderables.push_back(mRenderables[i]);
        }
    }

    mDefaultEffect->render(mViews, defaultRenderables);
    mSeaEffect->render(mViews, seaRenderables);
}

EFFECTS_NAMESPACE_END