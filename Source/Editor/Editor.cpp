#include "Editor/Editor.h"
#include "Scene/Scene.h"
#include "Renderer/Renderer.h"
#include "GamePlay/CameraController.h"
#include "Scene/Components/CCamera.h"
#include "Scene/Components/CRenderable.h"
#include "Renderer/RenderView.h"
#include "Renderer/Renderer.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "UI/UISystem.h"
#include "Editor/HierarchyTab.h"
#include "Editor/InspectorTab.h"
EFFECTS_NAMESPACE_BEGIN

Editor::Editor() {
}

Editor::~Editor() {
}

void Editor::init() {
    // tabs
    mHierarchyTab = std::make_shared<HierarchyTab>();
    mInspectorTab = std::make_shared<InspectorTab>();

    mEditCamera = et::SceneNode::create("MainCamera");
    et::CCamera* ccamera = mEditCamera->addComponent<et::CCamera>();
    ccamera->setViewPort(0.0f, 0.0f, 1200.0f, 1000.0f);
    ccamera->setFov(60.0f);
    ccamera->setNear(0.1f);
    ccamera->setFar(100.0f);
    ccamera->initialized();

    et::SceneNode* t0 = et::SceneNode::create("t0");
    et::SceneNode* t1 = et::SceneNode::create("t1");
    et::SceneNode* t2 = et::SceneNode::create("t2");
    t0->setParent(mEditCamera);
    t1->setParent(mEditCamera);
    t2->setParent(t0);

    et::SceneNode* cubeNode = et::SceneNode::create("cube");
    et::CRenderable* crenderable = cubeNode->addComponent<et::CRenderable>();
    std::shared_ptr<et::Mesh> cubeMesh = std::shared_ptr<et::Mesh>(et::genQuadMesh());
    std::shared_ptr<et::Material> cubeMaterial = std::make_shared<et::Material>();
    cubeMaterial->setType(et::EffectType::DEFAULT);
    crenderable->setMesh(cubeMesh);
    crenderable->setMaterial(cubeMaterial);
    crenderable->initialized();

    mEditCameraController = std::make_shared<et::CameraController>();
    mEditCameraController->setCamera(mEditCamera);
}

void Editor::update(float dt) {
    mEditCameraController->update(dt);
}

void Editor::render() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar;
    flags |= ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", 0, flags);
    ImGui::PopStyleVar();

    ImGuiID dockspaceID = ImGui::GetID("root");
    ImGui::DockSpace(dockspaceID);
    static bool loadDefaultLayout = true;
    if (loadDefaultLayout) {
        loadDefaultLayout = false;
        ImGui::DockBuilderRemoveNode(dockspaceID);
        ImGui::DockBuilderAddNode(dockspaceID, 0);
        ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->Size);

        ImGuiID dock_main_id = dockspaceID;
        ImGuiID dockHierarchy = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
        ImGuiID dockResources = ImGui::DockBuilderSplitNode(dockHierarchy, ImGuiDir_Down, 0.40f, nullptr, &dockHierarchy);
        ImGuiID dockInspector = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.30f, nullptr, &dock_main_id);
        ImGuiID dockLog = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.30f, nullptr, &dock_main_id);

        ImGui::DockBuilderDockWindow("hierarchy", dockHierarchy);
        ImGui::DockBuilderDockWindow("inspector", dockInspector);
        ImGui::DockBuilderDockWindow("resources", dockResources);
        ImGui::DockBuilderDockWindow("console", dockLog);
        ImGui::DockBuilderDockWindow("profiler", dockLog);
        ImGui::DockBuilderDockWindow("scene", dock_main_id);
        ImGui::DockBuilderFinish(dockspaceID);
    }

    mHierarchyTab->render();

    mInspectorTab->render();

    ImGui::Begin("resources");
    ImGui::End();

    ImGui::Begin("console");
    ImGui::End();

    ImGui::Begin("profiler");
    ImGui::End();

    ImGui::Begin("scene");
    et::CCamera* ccamera = mEditCamera->getComponent<et::CCamera>();

    static ImVec2 region;
    ImVec2 curRegion = ImGui::GetContentRegionAvail();
    if (region.x != curRegion.x || region.y != curRegion.y) {
        ccamera->setViewPort(0.0f, 0.0f, curRegion.x, curRegion.y);
        region = ImVec2(curRegion.x, curRegion.y);
    }
    ImGui::Image(reinterpret_cast<void*>(ccamera->getRenderView()->getColorTex()->handle), curRegion, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();

    ImGui::End();
    ImGui::PopStyleVar();
}

EFFECTS_NAMESPACE_END