#pragma once
#include "Core/Base.h"
#include <memory>
EFFECTS_NAMESPACE_BEGIN

class SceneNode;

enum class ComponentType {
    None,
    Renderable,
    Camera,
    Light,
    Sea
};

class Component
{
public:
    Component(SceneNode* node);
    ~Component();
    virtual void initialized() {}
    virtual void update(float dt) {}
    virtual void destroyed() {}
    virtual void onNodeDirty() {}
    static ComponentType getClassType() { return ComponentType::None; }
    virtual ComponentType getType() { return mType; }

protected:
    SceneNode* mNode;
    ComponentType mType;
};

EFFECTS_NAMESPACE_END