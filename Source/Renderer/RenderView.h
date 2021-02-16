#pragma once

#include "Core/Base.h"
#include "Core/Gfx/GfxResources.h"
#include "Math/Math.h"
#include <memory>
EFFECTS_NAMESPACE_BEGIN

class BaseEffect;
class Renderer;

class RenderView
{
public:
    RenderView();
    ~RenderView();
    void initialize();
    GfxFramebuffer* getRenderTarget() { return mRenderTarget; }
    GfxTexture* getColorTex() { return mColorTex; }
    GfxRenderbuffer* getRenderbuffer() { return mDepthTex; }
    void setFov(const float& fov);
    void setNear(const float& near);
    void setFar(const float& far);
    void setViewPort(const float& x, const float& y, const float& w, const float& h);
    float getFov();
    float getNear();
    float getFar();
    glm::vec4 getViewPort();
    glm::vec3 getPosition();
    void setTransform(const glm::mat4& mat);
    const glm::mat4& getTransform() const;
    const glm::mat4& getViewMatrix() const;
    const glm::mat4& getProjMatrix() const;
private:
    void resetRenderTarget();
private:
    friend Renderer;
    friend BaseEffect;
    GfxSampler* mSampler = nullptr;
    GfxTexture* mColorTex = nullptr;
    GfxRenderbuffer* mDepthTex = nullptr;
    GfxFramebuffer* mRenderTarget = nullptr;
    float mFov;
    float mNear;
    float mFar;
    glm::vec4 mViewPort;
    glm::mat4 mWorldMatrix;
    glm::mat4 mViewMatrix;
    glm::mat4 mProjMatrix;
    bool mProjDirtyFlag;
};


EFFECTS_NAMESPACE_END