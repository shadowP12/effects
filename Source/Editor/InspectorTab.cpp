#include "Editor/InspectorTab.h"
#include "Editor/Editor.h"
#include "Scene/Scene.h"
#include "Scene/Components/CCamera.h"
#include "Scene/Components/CRenderable.h"
#include "Renderer/RenderView.h"
#include "Renderer/Renderer.h"
#include "UI/UISystem.h"
EFFECTS_NAMESPACE_BEGIN

InspectorTab::InspectorTab() {
}

InspectorTab::~InspectorTab() {
}

void InspectorTab::render() {
    ImGui::Begin("inspector");
    if (Editor::instance().getSelectionNode() != nullptr) {
        SceneNode* node = Editor::instance().getSelectionNode();

        // Transform
        ImGui::Columns(2);

        ImGui::TextUnformatted("Position");
        ImGui::NextColumn();

        ImGui::PushItemWidth(-1);
        glm::vec3 position = node->getPosition();
        float min = -10000.0f;
        float max = 10000.0f;
        if (ImGui::DragScalarN("##Position", ImGuiDataType_Float, glm::value_ptr(position), 3, 0.2f, &min, &max, "%.3f")) {
            node->setPosition(position);
        }
        ImGui::PopItemWidth();
        ImGui::NextColumn();

        ImGui::TextUnformatted("Rotation");
        ImGui::NextColumn();

        ImGui::PushItemWidth(-1);
        glm::vec3 rotation = node->getEuler() * 180.0f / 3.1415926f;
        min = -10000.0f;
        max = 10000.0f;
        if (ImGui::DragScalarN("##Rotation", ImGuiDataType_Float, glm::value_ptr(rotation), 3, 0.2f, &min, &max, "%.3f")) {
            node->setEuler(glm::vec3(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z)));
        }
        ImGui::PopItemWidth();
        ImGui::NextColumn();

        ImGui::TextUnformatted("Scale");
        ImGui::NextColumn();

        ImGui::PushItemWidth(-1);
        glm::vec3 scale = node->getScale();
        min = -1000.0f;
        max = 1000.0f;
        if (ImGui::DragScalarN("##Scale", ImGuiDataType_Float, glm::value_ptr(scale), 3, 0.2f, &min, &max, "%.3f")) {
            node->setScale(scale);
        }
        ImGui::PopItemWidth();

        ImGui::Columns(1);
    }
    ImGui::End();
}

EFFECTS_NAMESPACE_END