#pragma once

#include "Core/Base.h"
EFFECTS_NAMESPACE_BEGIN

class SceneNode;

class HierarchyTab {
public:
    HierarchyTab();
    ~HierarchyTab();
    void render();
private:
    void renderNode(SceneNode* node);
};

EFFECTS_NAMESPACE_END
