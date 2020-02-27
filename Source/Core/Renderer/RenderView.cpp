#include "RenderView.h"
#include "Renderer.h"
EFFECTS_NAMESPACE_BEGIN
RenderView::RenderView()
{

}

RenderView::~RenderView()
{

}

void RenderView::initialize()
{
    Renderer::instance().notifyRenderViewAdded(shared_from_this());
}

void RenderView::setParams()
{

}

void RenderView::setTransform(const glm::mat4 &mat)
{
    mWorldMatrix = mat;
    mViewMatrix = glm::inverse(mWorldMatrix);
}

glm::mat4 RenderView::getTransform()
{
    return mWorldMatrix;
}

glm::mat4 RenderView::getViewMatrix()
{
    return mViewMatrix;
}

glm::mat4 RenderView::getProjMatrix()
{
    return mProjMatrix;
}

EFFECTS_NAMESPACE_END
