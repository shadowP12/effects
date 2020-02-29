#pragma once

#include "../Base.h"
#include "../../Math/Math.h"
#include <memory>
EFFECTS_NAMESPACE_BEGIN

class RenderView : public std::enable_shared_from_this<RenderView>
{
public:
    RenderView();
    ~RenderView();
    void initialize();
    void setFov(float fov);
    void setNear(float near);
    void setFar(float far);
    void setViewPort(float x, float y, float w, float h);
    void getFov(float fov);
    void getNear(float near);
    void getFar(float far);
    void getViewPort(float x, float y, float w, float h);
    void setTransform(const glm::mat4& mat);
    void setParams();
    glm::mat4 getTransform();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjMatrix();
private:
    float mFov;
    float mNear;
    float mFar;
    glm::vec4 mViewPort;
    glm::mat4 mWorldMatrix;
    glm::mat4 mViewMatrix;
    glm::mat4 mProjMatrix;
};


EFFECTS_NAMESPACE_END