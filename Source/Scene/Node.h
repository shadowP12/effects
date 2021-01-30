#pragma once
#include <memory>
#include <vector>
#include "Core/Base.h"
#include "Math/Math.h"

EFFECTS_NAMESPACE_BEGIN

class Node : public std::enable_shared_from_this<Node>
{
public:
	Node()
	{
		mParent = nullptr;
		mLPos = glm::vec3(0,0,0);
		mLScale = glm::vec3(1,1,1);
		mLRot = glm::quat(1,0,0,0);
        mPitch = 0.0f;
        mYaw = 0.0f;
        mRoll = 0.0;
        mLocalDirtyFlag = true;
        mWorldDirtyFlag = true;
	}
	virtual ~Node() 
	{
	}
	std::shared_ptr<Node> getParent() 
	{
		return mParent;
	}
	std::vector<std::shared_ptr<Node>> getChildren()
	{
		return mChildren;
	}
	void setParent(std::shared_ptr<Node> newParent);
	void appendChildren(std::shared_ptr<Node> newChildren);
	void removeChildren(std::shared_ptr<Node> children);
	void setTransform(const glm::vec3& pos, const glm::vec3& scale, const glm::quat& rot);
	glm::mat4 getLocalMatrix();
	glm::mat4 getWorldMatrix();
    void setPosition(const glm::vec3& pos);
    glm::vec3 getPosition(){return mLPos;}
    void setRotation(const glm::quat& rot);
    glm::quat getRotation(){return mLRot;}
    void setScale(const glm::vec3& scale);
    glm::vec3 getScale(){return mLScale;}
    glm::vec3 getRightVector();
    glm::vec3 getUpVector();
    glm::vec3 getFrontVector();
    void setPitch(const float& pitch);
    void setYaw(const float& yaw);
    void setRoll(const float& roll);
    float getPitch(){return mPitch;}
    float getYaw(){return mYaw;}
    float getRoll(){return mRoll;}
    void rotate(const glm::vec3 axis, const float& angle);
protected:
	std::shared_ptr<Node> mParent;
	std::vector<std::shared_ptr<Node>> mChildren;
	glm::vec3 mLPos;
	glm::vec3 mLScale;
	glm::quat mLRot;
    float mPitch;
    float mYaw;
    float mRoll;
	glm::mat4 mLocalMatrix;
    glm::mat4 mWorldMatrix;
    bool mLocalDirtyFlag;
    bool mWorldDirtyFlag;
};

EFFECTS_NAMESPACE_END