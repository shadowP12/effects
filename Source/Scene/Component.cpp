#include "Component.h"
#include "Scene.h"
EFFECTS_NAMESPACE_BEGIN

Component::Component(std::shared_ptr<SceneObject> so)
{
    mType = ComponentType::None;
    mSceneObject = so;
}

Component::~Component()
{}

EFFECTS_NAMESPACE_END