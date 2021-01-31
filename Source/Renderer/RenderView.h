#pragma once

#include "Core/Base.h"
#include "Core/Gfx/GfxResources.h"
#include "Math/Math.h"
#include <memory>
EFFECTS_NAMESPACE_BEGIN

class Renderer;

class RenderView
{
public:
    RenderView();
    ~RenderView();
    void initialize();
    void setFov(const float& fov);
    void setNear(const float& near);
    void setFar(const float& far);
    void setViewPort(const float& x, const float& y, const float& w, const float& h);
    float getFov();
    float getNear();
    float getFar();
    glm::vec4 getViewPort();
    void setTransform(const glm::mat4& mat);
    glm::mat4 getTransform();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjMatrix();
private:
    friend Renderer;
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