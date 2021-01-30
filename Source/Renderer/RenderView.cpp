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
    mProjDirtyFlag = true;
}

    glm::mat4 calculateLookAtMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
    {
        glm::vec3 zaxis = glm::normalize(position - target);
        glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
        glm::vec3 yaxis = glm::cross(zaxis, xaxis);

        glm::mat4 translation = glm::mat4(1.0f);
        translation[3][0] = -position.x;
        translation[3][1] = -position.y;
        translation[3][2] = -position.z;
        glm::mat4 rotation = glm::mat4(1.0f);
        rotation[0][0] = xaxis.x;
        rotation[1][0] = xaxis.y;
        rotation[2][0] = xaxis.z;
        rotation[0][1] = yaxis.x;
        rotation[1][1] = yaxis.y;
        rotation[2][1] = yaxis.z;
        rotation[0][2] = zaxis.x;
        rotation[1][2] = zaxis.y;
        rotation[2][2] = zaxis.z;
        return rotation * translation;
    }


void RenderView::setTransform(const glm::mat4 &mat)
{
    mWorldMatrix = mat;

    glm::vec3 position = getTranslate(mWorldMatrix);
    glm::vec3 front = getAxisZ(mWorldMatrix);
    glm::vec3 right = getAxisX(mWorldMatrix);
    glm::vec3 up = getAxisY(mWorldMatrix);

    mViewMatrix = calculateLookAtMatrix(position, position + front, up);
    //mViewMatrix = glm::inverse(mWorldMatrix);
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
    if(mProjDirtyFlag)
    {
        mProjDirtyFlag = false;
        float ratio = (mViewPort.z - mViewPort.x) / (mViewPort.w - mViewPort.y);
        mProjMatrix = glm::perspective(glm::radians(mFov), ratio, mNear, mFar);
    }
    return mProjMatrix;
}

void RenderView::setFov(const float& fov)
{
    mFov = fov;
    mProjDirtyFlag = true;
}

void RenderView::setNear(const float& near)
{
    mNear = near;
    mProjDirtyFlag = true;
}

void RenderView::setFar(const float& far)
{
    mFar = far;
    mProjDirtyFlag = true;
}

void RenderView::setViewPort(const float& x, const float& y, const float& w, const float& h)
{
    mViewPort = glm::vec4(x, y, w, h);
    mProjDirtyFlag = true;
}

float RenderView::getFov()
{
    return mFov;
}

float RenderView::getNear()
{
    return mNear;
}

float RenderView::getFar()
{
    return mFar;
}

glm::vec4 RenderView::getViewPort()
{
    return mViewPort;
}

EFFECTS_NAMESPACE_END
