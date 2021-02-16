#include "RenderView.h"
#include "Renderer.h"
EFFECTS_NAMESPACE_BEGIN
RenderView::RenderView() {
}

RenderView::~RenderView() {
    SAFE_DELETE(mSampler);
    SAFE_DELETE(mColorTex);
    SAFE_DELETE(mDepthTex);
    SAFE_DELETE(mRenderTarget);
}

void RenderView::initialize() {
}

void RenderView::resetRenderTarget() {
    // todo
    SAFE_DELETE(mSampler);
    SAFE_DELETE(mColorTex);
    SAFE_DELETE(mDepthTex);
    SAFE_DELETE(mRenderTarget);

    GfxSamplerDesc samplerDesc;
    samplerDesc.minFilter = GL_LINEAR;
    samplerDesc.magFilter = GL_LINEAR;
    samplerDesc.wrapS = GL_REPEAT;
    samplerDesc.wrapT = GL_REPEAT;
    mSampler = createGfxSampler(samplerDesc);

    GfxTextureDesc drawingTexDesc;
    drawingTexDesc.width = mViewPort.z;
    drawingTexDesc.height = mViewPort.w;
    drawingTexDesc.componentType = GL_FLOAT;
    drawingTexDesc.internalFormat = GL_RGBA16;
    drawingTexDesc.format = GL_RGBA;
    mColorTex = createGfxTexture(drawingTexDesc);
    writeGfxTextureData(mColorTex, nullptr);
    setGfxTextureSampler(mColorTex, mSampler);

    GfxRenderbufferDesc drawingDepthBufferDesc;
    drawingDepthBufferDesc.width = mViewPort.z;
    drawingDepthBufferDesc.height = mViewPort.w;
    drawingDepthBufferDesc.internalformat = GL_DEPTH_COMPONENT;
    mDepthTex = createGfxRenderbuffer(drawingDepthBufferDesc);

    GfxFramebufferDesc drawingFramebufferDesc;
    drawingFramebufferDesc.targets[0] = mColorTex;
    drawingFramebufferDesc.depthBuffer = mDepthTex;
    mRenderTarget = createGfxFramebuffer(drawingFramebufferDesc);
}

void RenderView::setTransform(const glm::mat4 &mat)
{
    mWorldMatrix = mat;

    glm::vec3 position = getTranslate(mWorldMatrix);
    glm::vec3 front = getAxisZ(mWorldMatrix);
    glm::vec3 right = getAxisX(mWorldMatrix);
    glm::vec3 up = getAxisY(mWorldMatrix);

    mViewMatrix = glm::lookAt(position, position + front, up);
    //mViewMatrix = glm::inverse(mWorldMatrix);
}

const glm::mat4& RenderView::getTransform() const
{
    return mWorldMatrix;
}

const glm::mat4& RenderView::getViewMatrix() const {
    return mViewMatrix;
}

const glm::mat4& RenderView::getProjMatrix() const {
    return mProjMatrix;
}

void RenderView::setFov(const float& fov)
{
    mFov = fov;
    float ratio = (mViewPort.z - mViewPort.x) / (mViewPort.w - mViewPort.y);
    mProjMatrix = glm::perspective(glm::radians(mFov), ratio, mNear, mFar);
}

void RenderView::setNear(const float& near)
{
    mNear = near;
    float ratio = (mViewPort.z - mViewPort.x) / (mViewPort.w - mViewPort.y);
    mProjMatrix = glm::perspective(glm::radians(mFov), ratio, mNear, mFar);
}

void RenderView::setFar(const float& far)
{
    mFar = far;
    float ratio = (mViewPort.z - mViewPort.x) / (mViewPort.w - mViewPort.y);
    mProjMatrix = glm::perspective(glm::radians(mFov), ratio, mNear, mFar);
}

void RenderView::setViewPort(const float& x, const float& y, const float& w, const float& h)
{
    mViewPort = glm::vec4(x, y, w, h);
    resetRenderTarget();
    float ratio = (mViewPort.z - mViewPort.x) / (mViewPort.w - mViewPort.y);
    mProjMatrix = glm::perspective(glm::radians(mFov), ratio, mNear, mFar);
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

glm::vec3 RenderView::getPosition() {
    return getTranslate(mWorldMatrix);
}

EFFECTS_NAMESPACE_END
