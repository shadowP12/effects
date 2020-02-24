#pragma once
#include <memory>
#include <vector>
#include "../Base.h"
#include "../../Math/Math.h"

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
	void removeChildren(std::shared_ptr<Node> Children);
	void setTransform(const glm::vec3& pos, const glm::vec3& scale, const glm::quat& rot);
	glm::mat4 getLocalMatrix();
	glm::mat4 getWorldMatrix();
protected:
	std::shared_ptr<Node> mParent;
	std::vector<std::shared_ptr<Node>> mChildren;
	glm::vec3 mLPos;
	glm::vec3 mLScale;
	glm::quat mLRot;
};

EFFECTS_NAMESPACE_END