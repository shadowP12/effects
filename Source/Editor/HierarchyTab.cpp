#include "Editor/HierarchyTab.h"
#include "Editor/Editor.h"
#include "Scene/Scene.h"
#include "Scene/Components/CCamera.h"
#include "Scene/Components/CRenderable.h"
#include "Renderer/RenderView.h"
#include "Renderer/Renderer.h"
#include "UI/UISystem.h"
#include <stack>
EFFECTS_NAMESPACE_BEGIN

HierarchyTab::HierarchyTab() {
}

HierarchyTab::~HierarchyTab() {
}

void HierarchyTab::render() {
    ImGui::Begin("hierarchy");

    for (int i = 0; i < et::Scene::instance().getNodes().size(); ++i) {
        et::SceneNode* node = et::Scene::instance().getNodes()[i];
        if (node->getParent() == nullptr) {
            renderNode(node);
        }
    }
    ImGui::End();
}

void HierarchyTab::renderNode(SceneNode* node) {
    ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
    treeNodeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth;
    if (Editor::instance().getSelectionNode() == node) {
        treeNodeFlags |= ImGuiTreeNodeFlags_Selected;
    }
    bool opened = ImGui::TreeNodeEx((void*)node, treeNodeFlags, node->getId().c_str());
    if (ImGui::IsItemClicked()) {
        Editor::instance().setSelectionTab(SelectionTabType::HIERARCHY);
        Editor::instance().setSelectionNode(node);
    }

    if (opened) {
        for (int i = 0; i < node->getChildren().size(); ++i) {
            SceneNode* child = node->getChildren()[i];
            renderNode(child);
        }
        ImGui::TreePop();
    }
}

EFFECTS_NAMESPACE_END
