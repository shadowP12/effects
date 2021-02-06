#pragma once
#include <memory>
#include <vector>
#include "Core/Base.h"
#include "Core/Utility/Log.h"
#include "Math/Math.h"

EFFECTS_NAMESPACE_BEGIN

class Node
{
public:
	Node() {
		mParent = nullptr;
		mLPos = glm::vec3(0,0,0);
		mLScale = glm::vec3(1,1,1);
		mLRot = glm::quat(1,0,0,0);
        mDirtyFlag = true;
	}

	virtual ~Node() {
        setParent(nullptr);

        std::vector<Node*> temp = mChildren;
        for (int i = 0; i < temp.size(); ++i) {
            SAFE_DELETE(temp[i]);
        }
		mChildren.clear();
	}

	Node* getParent() {
		return mParent;
	}

	std::vector<Node*>& getChildren() {
		return mChildren;
	}
	void setParent(Node* newParent);
	void appendChildren(Node* newChildren);
	void removeChildren(Node* oldChildren);
	void setTransform(const glm::vec3& pos, const glm::vec3& scale, const glm::quat& rot);
	glm::mat4 getLocalMatrix();
	glm::mat4 getWorldMatrix();
    void setPosition(const glm::vec3& pos);
    glm::vec3 getPosition(){ return mLPos; }
    void setRotation(const glm::quat& rot);
    glm::quat getRotation(){ return mLRot; }
    void setScale(const glm::vec3& scale);
    glm::vec3 getScale(){ return mLScale; }
    glm::vec3 getRightVector();
    glm::vec3 getUpVector();
    glm::vec3 getFrontVector();
    void rotate(const glm::vec3 axis, const float& angle);
protected:
	Node* mParent;
	std::vector<Node*> mChildren;
	glm::vec3 mLPos;
	glm::vec3 mLScale;
	glm::quat mLRot;
	glm::mat4 mLocalMatrix;
    glm::mat4 mWorldMatrix;
    bool mDirtyFlag;
};

EFFECTS_NAMESPACE_END