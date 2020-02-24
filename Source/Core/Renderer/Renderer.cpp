#include "Renderer.h"
EFFECTS_NAMESPACE_BEGIN

Renderer::Renderer()
{}

Renderer::~Renderer()
{}

void Renderer::notifyRenderableAdded(std::shared_ptr<Renderable> renderable)
{
    mRenderables.push_back(renderable);
}

EFFECTS_NAMESPACE_END