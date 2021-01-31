#include "Node.h"
EFFECTS_NAMESPACE_BEGIN

void Node::setParent(Node* newParent) {
	Node* oldParent = mParent;

	if (oldParent == newParent)
		return;

	Node* curNode = newParent;
	while (curNode) {
		if (curNode == this)
			return;
		curNode = curNode->getParent();
	}

	if (oldParent) {
		for (auto iter = oldParent->mChildren.begin(); iter != oldParent->mChildren.end(); ) {
			if (iter == oldParent->mChildren.end())
				break;
			if (*iter == this)
				oldParent->mChildren.erase(iter);
		}
	}

	mParent = newParent;
	if (newParent) {
		newParent->mChildren.push_back(this);
	}
	mDirtyFlag = true;
}

void Node::appendChildren(Node* newChildren) {
	if (!newChildren)
		return;

	Node* curNode = this;
	while (curNode)
	{
		if (curNode == newChildren)
			return;
		curNode = curNode->getParent();
	}

	Node* oldParent = newChildren->mParent;
	if (oldParent) {
		for (auto iter = oldParent->mChildren.begin(); iter != oldParent->mChildren.end(); ) {
			if (iter == oldParent->mChildren.end())
				break;
			if (*iter == newChildren)
				oldParent->mChildren.erase(iter);
		}
	}
	
	newChildren->mParent = this;
	newChildren->mDirtyFlag = true;
	mChildren.push_back(newChildren);
}

void Node::removeChildren(Node* children) {
	for (auto iter = mChildren.begin(); iter != mChildren.end(); ) {
		if (iter == mChildren.end())
			break;

		if (*iter == children) {
			mChildren.erase(iter);
            children->mParent = nullptr;
            children->mDirtyFlag = true;
		}
	}
}

void Node::setTransform(const glm::vec3 &pos, const glm::vec3 &scale, const glm::quat &rot) {
    mLPos = pos;
    mLScale = scale;
    mLRot = rot;
    mDirtyFlag = true;
}

void Node::setPosition(const glm::vec3& pos) {
    mLPos = pos;
    mDirtyFlag = true;
    for (int i = 0; i < mChildren.size(); ++i) {
        mChildren[i]->mDirtyFlag = true;
    }
}

void Node::setRotation(const glm::quat& rot) {
    mLRot = rot;
    mDirtyFlag = true;
    for (int i = 0; i < mChildren.size(); ++i) {
        mChildren[i]->mDirtyFlag = true;
    }
}

void Node::setScale(const glm::vec3& scale) {
    mLScale = scale;
    mDirtyFlag = true;
    for (int i = 0; i < mChildren.size(); ++i) {
        mChildren[i]->mDirtyFlag = true;
    }
}

void Node::setPitch(const float& pitch) {
    mPitch = pitch;
    mLRot = glm::quat(glm::vec3(glm::radians(mPitch), glm::radians(mYaw), glm::radians(mRoll)));//fromEulerAngles(mPitch, mYaw, mRoll);
    float pp = glm::pitch(mLRot);
    pp = pp * 180.0f / PI;
    mDirtyFlag = true;
    for (int i = 0; i < mChildren.size(); ++i) {
        mChildren[i]->mDirtyFlag = true;
    }
}

void Node::setYaw(const float& yaw) {
    mYaw = yaw;
    mLRot = glm::quat(glm::vec3(glm::radians(mPitch), glm::radians(mYaw), glm::radians(mRoll)));
    //mLRot = fromEulerAngles(glm::radians(mPitch), glm::radians(mYaw), glm::radians(mRoll));
    mDirtyFlag = true;
    for (int i = 0; i < mChildren.size(); ++i) {
        mChildren[i]->mDirtyFlag = true;
    }
}

void Node::setRoll(const float& roll) {
    mRoll = roll;
    mLRot = glm::quat(glm::vec3(glm::radians(mPitch), glm::radians(mYaw), glm::radians(mRoll)));
    //mLRot = fromEulerAngles(glm::radians(mPitch), glm::radians(mYaw), glm::radians(mRoll));
    mDirtyFlag = true;
    for (int i = 0; i < mChildren.size(); ++i) {
        mChildren[i]->mDirtyFlag = true;
    }
}

void Node::rotate(const glm::vec3 axis, const float &angle) {
    glm::quat r = fromAxisAngle(axis, glm::radians(angle));
    mLRot = mLRot * r;
    mDirtyFlag = true;
    for (int i = 0; i < mChildren.size(); ++i) {
        mChildren[i]->mDirtyFlag = true;
    }
}

glm::mat4 Node::getLocalMatrix()
{
    //RTS
    glm::mat4 R, T, S;
    R = glm::toMat4(mLRot);
    T = glm::translate(glm::mat4(1.0),mLPos);
    S = glm::scale(glm::mat4(1.0),mLScale);
    mLocalMatrix = T * R * S;
    return mLocalMatrix;
}

glm::mat4 Node::getWorldMatrix()
{
    glm::mat4 out = getLocalMatrix();
    Node* cur = mParent;
    while (cur) {
        out = cur->getLocalMatrix() * out;
        cur = cur->mParent;
    }
    mWorldMatrix = out;
    return mWorldMatrix;
}

glm::vec3 Node::getRightVector() {
    return getAxisX(getWorldMatrix());
}

glm::vec3 Node::getUpVector() {
    return getAxisY(getWorldMatrix());
}

glm::vec3 Node::getFrontVector() {
    return getAxisZ(getWorldMatrix());
}

EFFECTS_NAMESPACE_END