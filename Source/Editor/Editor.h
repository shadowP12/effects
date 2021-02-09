#pragma once
#include <vector>
#include <map>
#include <memory>
#include "Core/Base.h"
#include "Core/Utility/Module.h"
EFFECTS_NAMESPACE_BEGIN

class SceneNode;
class CameraController;

class Editor : public Module<Editor>
{
public:
    Editor();
    ~Editor();
    void init();
    void update(float dt);
    void render();
    void setSelectionNode(SceneNode* node) { mSelectionNode = node; }
    SceneNode* getSelectionNode() { return mSelectionNode; }
private:
    SceneNode* mEditCamera = nullptr;
    SceneNode* mSelectionNode = nullptr;
    std::shared_ptr<CameraController> mEditCameraController;
};

EFFECTS_NAMESPACE_END