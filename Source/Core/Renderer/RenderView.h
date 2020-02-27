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
    void setTransform(const glm::mat4& mat);
    void setParams();
    glm::mat4 getTransform();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjMatrix();
private:
    glm::mat4 mWorldMatrix;
    glm::mat4 mViewMatrix;
    glm::mat4 mProjMatrix;
};


EFFECTS_NAMESPACE_END