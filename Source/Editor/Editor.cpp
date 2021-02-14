#include "Editor/Editor.h"
#include "Scene/Scene.h"
#include "Renderer/Renderer.h"
#include "GamePlay/CameraController.h"
#include "Scene/Components/CCamera.h"
#include "Scene/Components/CRenderable.h"
#include "Scene/Components/CLight.h"
#include "Renderer/RenderView.h"
#include "Renderer/Renderer.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "UI/UISystem.h"
#include "Editor/HierarchyTab.h"
#include "Editor/InspectorTab.h"
#include "Editor/ResourceTab.h"
#include "Importers/GltfImporter.h"
#include "Core/Utility/Log.h"

#include "GLFW/glfw3native.h"
#include <windows.h>
#include <commdlg.h>
#include <string>
EFFECTS_NAMESPACE_BEGIN

bool Editor::openFile(const std::string& filter, std::string& file) {
    OPENFILENAMEA ofn;
    CHAR szFile[260] = { 0 };
    CHAR currentDir[256] = { 0 };
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = static_cast<HWND>(mWindowPtr);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    if (GetCurrentDirectoryA(256, currentDir))
        ofn.lpstrInitialDir = currentDir;
    ofn.lpstrFilter = filter.c_str();
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn) == TRUE) {
        file = ofn.lpstrFile;
        return true;
    }
    return false;
}

Editor::Editor() {
}

Editor::~Editor() {
}

void Editor::init(void* windowPtr) {
    mWindowPtr = windowPtr;
    // importers
    mGltfImporter = std::make_shared<GltfImporter>();

    // tabs
    mHierarchyTab = std::make_shared<HierarchyTab>();
    mInspectorTab = std::make_shared<InspectorTab>();
    mResourceTab = std::make_shared<ResourceTab>();

    mEditCamera = et::SceneNode::create("MainCamera");
    et::CCamera* ccamera = mEditCamera->addComponent<et::CCamera>();
    ccamera->setViewPort(0.0f, 0.0f, 1200.0f, 1000.0f);
    ccamera->setFov(60.0f);
    ccamera->setNear(0.1f);
    ccamera->setFar(100.0f);
    ccamera->initialized();

//    et::SceneNode* cubeNode = et::SceneNode::create("cube");
//    et::CRenderable* crenderable = cubeNode->addComponent<et::CRenderable>();
//    std::shared_ptr<et::Mesh> cubeMesh = et::genCubeMesh();
//    std::shared_ptr<et::Material> cubeMaterial = Material::create("Test_Material");
//    cubeMaterial->setEffectType(et::EffectType::DEFAULT);
//    crenderable->setMesh(cubeMesh);
//    crenderable->setMaterial(cubeMaterial);
//    crenderable->initialized();

    et::SceneNode* sunNode = et::SceneNode::create("Sun");
    sunNode->setEuler(glm::vec3(3.1415926 / 2.0f, 0.0f, 0.0f));
    et::CLight* clight = sunNode->addComponent<et::CLight>();
    clight->setLightType(RenderLightType::DIRECT_LIGHT);
    clight->setLightColorAndIntensity(glm::vec4(0.9, 0.9, 0.9, 2.5));

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
    flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;
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

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                std::string filePath;
                if (openFile("", filePath)) {
                    if (!mGltfImporter->load(filePath)) {
                        LOGE("Load filed : %s\n", filePath.c_str());
                    }
                }
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    mHierarchyTab->render();

    mInspectorTab->render();

    mResourceTab->render();

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