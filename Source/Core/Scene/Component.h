#include "../Base.h"
#include <memory>
EFFECTS_NAMESPACE_BEGIN

class SceneObject;

enum class ComponentType
{
    None,
    Renderable,
    Camera,
};

class Component
{
public:
    Component(std::shared_ptr<SceneObject> so);
    ~Component();
    virtual void initialized() {}
    virtual void update() {}
    virtual void destroyed() {}
    static ComponentType getClassType(){return ComponentType::None;}
    virtual ComponentType getType(){return mType;}
protected:
    std::shared_ptr<SceneObject> mSceneObject;
    ComponentType mType;
};

EFFECTS_NAMESPACE_END