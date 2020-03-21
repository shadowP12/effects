#include "Node.h"
EFFECTS_NAMESPACE_BEGIN

void Node::setParent(std::shared_ptr<Node> newParent)
{
	std::shared_ptr<Node> oldParent = mParent;

	if (oldParent == newParent)
		return;

	std::shared_ptr<Node> curNode = newParent;
	while (curNode)
	{
		if (curNode == shared_from_this())
			return;
		curNode = curNode->getParent();
	}

	if (oldParent)
	{
		for (std::vector<std::shared_ptr<Node>>::iterator iter = oldParent->mChildren.begin(); iter != oldParent->mChildren.end(); )
		{
			if (iter == oldParent->mChildren.end())
				break;
			if (*iter == shared_from_this())
				oldParent->mChildren.erase(iter);
		}
	}

	mParent = newParent;
	if (newParent)
	{
		newParent->mChildren.push_back(shared_from_this());
	}
	mWorldDirtyFlag = true;
}

void Node::appendChildren(std::shared_ptr<Node> newChildren) 
{
	if (!newChildren)
		return;

	std::shared_ptr<Node> curNode = std::dynamic_pointer_cast<Node>(shared_from_this());
	while (curNode)
	{
		if (curNode == newChildren)
			return;
		curNode = curNode->getParent();
	}

	std::shared_ptr<Node> oldParent = newChildren->mParent;
	if (oldParent)
	{
		for (std::vector<std::shared_ptr<Node>>::iterator iter = oldParent->mChildren.begin(); iter != oldParent->mChildren.end(); )
		{
			if (iter == oldParent->mChildren.end())
				break;
			if (*iter == newChildren)
				oldParent->mChildren.erase(iter);
		}
	}
	
	newChildren->mParent = shared_from_this();
	newChildren->mWorldDirtyFlag = true;
	mChildren.push_back(newChildren);
}

void Node::removeChildren(std::shared_ptr<Node> children)
{
	for (std::vector<std::shared_ptr<Node>>::iterator iter = mChildren.begin(); iter != mChildren.end(); )
	{
		if (iter == mChildren.end())
			break;
		if (*iter == children)
		{
			mChildren.erase(iter);
            children->mParent = NULL;
            children->mWorldDirtyFlag = true;
		}
	}
}

void Node::setTransform(const glm::vec3 &pos, const glm::vec3 &scale, const glm::quat &rot)
{
    mLPos = pos;
    mLScale = scale;
    mLRot = rot;
    mLocalDirtyFlag = true;
}

glm::mat4 Node::getLocalMatrix()
{
    if(1)
    {
        //RTS
        glm::mat4 R, T, S;
        R = glm::toMat4(mLRot);
        T = glm::translate(glm::mat4(1.0),mLPos);
        S = glm::scale(glm::mat4(1.0),mLScale);
        mLocalMatrix = T * R * S;
        mLocalDirtyFlag = false;
    }
    return mLocalMatrix;
}

glm::mat4 Node::getWorldMatrix()
{
    if(1)
    {
        glm::mat4 out = getLocalMatrix();
        std::shared_ptr<Node> cur = mParent;
        while (cur)
        {
            out = cur->getLocalMatrix() * out;
        }
        mWorldMatrix = out;
        mWorldDirtyFlag = false;
    }
    return mWorldMatrix;
}

void Node::setPosition(const glm::vec3& pos)
{
    mLPos = pos;
    mLocalDirtyFlag = true;
}

void Node::setRotation(const glm::quat& rot)
{
    mLRot = rot;
    mLocalDirtyFlag = true;
}

void Node::setScale(const glm::vec3& scale)
{
    mLScale = scale;
    mLocalDirtyFlag = true;
}

glm::vec3 Node::getRightVector()
{
    return getAxisX(getWorldMatrix());
}

glm::vec3 Node::getUpVector()
{
    return getAxisY(getWorldMatrix());
}

glm::vec3 Node::getFrontVector()
{
    return getAxisZ(getWorldMatrix());
}

void Node::setPitch(const float& pitch)
{
    mPitch = pitch;
    mLRot = glm::quat(glm::vec3(glm::radians(mPitch), glm::radians(mYaw), glm::radians(mRoll)));//fromEulerAngles(mPitch, mYaw, mRoll);
    float pp = glm::pitch(mLRot);
    pp = pp * 180.0f / PI;
    mLocalDirtyFlag = true;
}

void Node::setYaw(const float& yaw)
{
    mYaw = yaw;
    mLRot = glm::quat(glm::vec3(glm::radians(mPitch), glm::radians(mYaw), glm::radians(mRoll)));
    //mLRot = fromEulerAngles(glm::radians(mPitch), glm::radians(mYaw), glm::radians(mRoll));
    mLocalDirtyFlag = true;
}

void Node::setRoll(const float& roll)
{
    mRoll = roll;
    mLRot = glm::quat(glm::vec3(glm::radians(mPitch), glm::radians(mYaw), glm::radians(mRoll)));
    //mLRot = fromEulerAngles(glm::radians(mPitch), glm::radians(mYaw), glm::radians(mRoll));
    mLocalDirtyFlag = true;
}

void Node::rotate(const glm::vec3 axis, const float &angle)
{
    glm::quat r = fromAxisAngle(axis, glm::radians(angle));
    mLRot = mLRot * r;
    mLocalDirtyFlag = true;
}

EFFECTS_NAMESPACE_END