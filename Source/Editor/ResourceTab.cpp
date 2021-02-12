#include "Editor/ResourceTab.h"
#include "Editor/Editor.h"
#include "Scene/Scene.h"
#include "Scene/Components/CCamera.h"
#include "Scene/Components/CRenderable.h"
#include "Renderer/RenderView.h"
#include "Renderer/Renderer.h"
#include "UI/UISystem.h"
#include "Resources/ResourceManager.h"
EFFECTS_NAMESPACE_BEGIN

ResourceTab::ResourceTab() {
}

ResourceTab::~ResourceTab() {
}

void ResourceTab::render() {
    ImGui::Begin("resources");
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1)) {
        ImGui::OpenPopup("ResourceOperate");
    }

    if (ImGui::BeginPopup("ResourceOperate"))
    {
        if (ImGui::Selectable("addSea")) {

        }
        ImGui::EndPopup();
    }

    std::map<std::string, std::shared_ptr<Resource>> rs = ResourceManager::instance().getAllRes();
    for (auto& var : rs) {
        std::shared_ptr<Resource> res = var.second;
        ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth;
        if (Editor::instance().getSelectionResource() == res) {
            treeNodeFlags |= ImGuiTreeNodeFlags_Selected;
        }
        bool opened = ImGui::TreeNodeEx(res->getUUID().c_str(), treeNodeFlags, res->getId().c_str());

        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
            Editor::instance().setSelectionResource(res);
        }

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            Editor::instance().setSelectionTab(SelectionTabType::RESOURCE);
        }

        if (opened) {
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

EFFECTS_NAMESPACE_END