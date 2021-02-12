#pragma once
#include <vector>
#include <map>
#include <memory>
#include "Core/Base.h"
#include "Core/Utility/Module.h"
EFFECTS_NAMESPACE_BEGIN

class SceneNode;
class Resource;
class CameraController;
class HierarchyTab;
class InspectorTab;
class ResourceTab;

enum class SelectionTabType {
    NONE = 0,
    RESOURCE,
    HIERARCHY,
    SCENE
};

class Editor : public Module<Editor>
{
public:
    Editor();
    ~Editor();
    void init();
    void update(float dt);
    void render();
    void setSelectionTab(SelectionTabType type) { mSelectionTab = type; }
    SelectionTabType getSelectionTab() { return mSelectionTab; }
    void setSelectionNode(SceneNode* node) { mSelectionNode = node; }
    SceneNode* getSelectionNode() { return mSelectionNode; }
    void setSelectionResource(std::shared_ptr<Resource> res) { mSelectionResource = res; }
    std::shared_ptr<Resource> getSelectionResource() { return mSelectionResource; }
private:
    SceneNode* mEditCamera = nullptr;
    SelectionTabType mSelectionTab = SelectionTabType::NONE;
    SceneNode* mSelectionNode = nullptr;
    std::shared_ptr<Resource> mSelectionResource = nullptr;
    std::shared_ptr<CameraController> mEditCameraController;
    std::shared_ptr<HierarchyTab> mHierarchyTab;
    std::shared_ptr<InspectorTab> mInspectorTab;
    std::shared_ptr<ResourceTab> mResourceTab;
};

EFFECTS_NAMESPACE_END