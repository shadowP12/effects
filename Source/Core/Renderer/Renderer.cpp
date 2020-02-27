#include "Renderer.h"
#include "Renderable.h"
#include "RenderView.h"
#include "../RenderResources/Mesh.h"
#include "../RenderResources/Material.h"
EFFECTS_NAMESPACE_BEGIN

Renderer::Renderer()
{}

Renderer::~Renderer()
{}

void Renderer::notifyRenderViewAdded(std::shared_ptr<RenderView> view)
{
    mMainView = view;
}

void Renderer::notifyRenderableAdded(std::shared_ptr<Renderable> renderable)
{
    mRenderables.push_back(renderable);
}

void Renderer::render()
{
    // seting global defines

    for (int i = 0; i < mRenderables.size(); ++i)
    {
        std::shared_ptr<Mesh> mesh = mRenderables[i]->getMesh();
        std::shared_ptr<Material> material = mRenderables[i]->getMaterial();
        material->setProgramDefines(mGlobalDefines);
        GpuProgram* program = material->getProgram();
        GLuint programID = program->getGpuProgram(mGlobalDefines);
    }
}

EFFECTS_NAMESPACE_END