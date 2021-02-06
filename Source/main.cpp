#include <iostream>
#include "Core/Gfx/Gfx.h"
#include "UI/UISystem.h"
#include "Renderer/RenderView.h"
#include "Renderer/Renderer.h"
#include "RenderResources/Mesh.h"
#include "RenderResources/Material.h"
#include "Importers/GltfImporter.h"
#include "Core/InputSystem/InputSystem.h"
#include "Core/Utility/FileUtility.h"
#include "Core/Utility/Event.h"
#include "Core/Utility/Hash.h"
#include "Scene/Scene.h"
#include "Scene/Components/CCamera.h"
#include "Scene/Components/CRenderable.h"
#include "GamePlay/CameraController.h"
#define SCREEN_WIDTH 800 
#define SCREEN_HEIGHT 600

et::SceneNode* gMainCamera = nullptr;
std::shared_ptr<et::CameraController> gEditCameraController;
GLFWwindow* g_window = nullptr;

// events
Event<void, float, float> onMousePositionEvent;
Event<void, int, int> onMouseButtonEvent;
Event<void, float> onMouseScrollEvent;
Event<void, int, int> onWindowResizeEvent;
Event<void> onFrameFinishEvent;

void cursor_pos_callback(GLFWwindow * window, double pos_x, double pos_y);
void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);
void mouse_scroll_callback(GLFWwindow * window, double offset_x, double offset_y);
void window_size_callback(GLFWwindow* window, int width, int height);

void init()
{
	// module
	et::InputSystem::startUp();
    onMousePositionEvent.bind(CALLBACK_2(et::InputSystem::onMousePosition, et::InputSystem::instancePtr()));
    onMouseButtonEvent.bind(CALLBACK_2(et::InputSystem::onMouseButton, et::InputSystem::instancePtr()));
    onMouseScrollEvent.bind(CALLBACK_1(et::InputSystem::onMouseScroll, et::InputSystem::instancePtr()));
    onFrameFinishEvent.bind(CALLBACK_0(et::InputSystem::reset, et::InputSystem::instancePtr()));

	et::Renderer::startUp();
	et::Scene::startUp();

    {
        gMainCamera = et::Scene::instance().addNode();
        et::CCamera* ccamera = gMainCamera->addComponent<et::CCamera>();
        ccamera->setViewPort(0.0f, 0.0f, 1200.0f, 1000.0f);
        ccamera->setFov(60.0f);
        ccamera->setNear(0.1f);
        ccamera->setFar(100.0f);
        ccamera->initialized();
    }

    {
        et::SceneNode* cubeNode = et::Scene::instance().addNode();
        et::CRenderable* crenderable = cubeNode->addComponent<et::CRenderable>();
        std::shared_ptr<et::Mesh> cubeMesh = std::shared_ptr<et::Mesh>(et::genQuadMesh());
        std::shared_ptr<et::Material> cubeMaterial = std::make_shared<et::Material>();
        cubeMaterial->setType(et::EffectType::DEFAULT);
        crenderable->setMesh(cubeMesh);
        crenderable->setMaterial(cubeMaterial);
        crenderable->initialized();
    }

    gEditCameraController = std::make_shared<et::CameraController>();
    gEditCameraController->setCamera(gMainCamera);
}

void release() {
	et::InputSystem::shutDown();
	et::Renderer::shutDown();
	et::Scene::shutDown();
}

void update(float t) {
    gEditCameraController->update(t);
	et::Scene::instance().update(t);
}

int main()
{
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	g_window = glfwCreateWindow(1200, 800, "effects", NULL, NULL);
	if (g_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(g_window);
	glfwSetMouseButtonCallback(g_window, mouse_button_callback);
	glfwSetCursorPosCallback(g_window, cursor_pos_callback);
	glfwSetScrollCallback(g_window, mouse_scroll_callback);
	glfwSetFramebufferSizeCallback(g_window, window_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(g_window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    bool loadDefaultLayout = true;
    ImVec2 region = ImVec2(0.0f, 0.0f);
	while (!glfwWindowShouldClose(g_window)) {
        glfwPollEvents();
        update(0.001);
        et::Renderer::instance().render();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

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
            ImGui::DockBuilderDockWindow("resources", dockResources);
            ImGui::DockBuilderDockWindow("console", dockLog);
            ImGui::DockBuilderDockWindow("profiler", dockLog);
            ImGui::DockBuilderDockWindow("scene", dock_main_id);
            ImGui::DockBuilderDockWindow("inspector", dockInspector);
            ImGui::DockBuilderFinish(dockspaceID);
        }

        ImGui::Begin("hierarchy");
        ImGui::End();

        ImGui::Begin("resources");
        ImGui::End();

        ImGui::Begin("console");
        ImGui::End();

        ImGui::Begin("profiler");
        ImGui::End();

        ImGui::Begin("scene");
        et::CCamera* ccamera = gMainCamera->getComponent<et::CCamera>();

        ImVec2 curRegion = ImGui::GetContentRegionAvail();
        if (region.x != curRegion.x || region.y != curRegion.y) {
            ccamera->setViewPort(0.0f, 0.0f, curRegion.x, curRegion.y);
            region = ImVec2(curRegion.x, curRegion.y);
        }
        ImGui::Image(reinterpret_cast<void*>(ccamera->getRenderView()->getColorTex()->handle), curRegion);
        ImGui::End();

        ImGui::Begin("inspector");
        ImGui::End();

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

		glfwSwapBuffers(g_window);
		onFrameFinishEvent.dispatch();
	}

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    release();
	glfwDestroyWindow(g_window);
	glfwTerminate();
	return 0;
}

void cursor_pos_callback(GLFWwindow * window, double pos_x, double pos_y)
{
    onMousePositionEvent.dispatch(pos_x, pos_y);
}

void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
    onMouseButtonEvent.dispatch(button, action);
}

void mouse_scroll_callback(GLFWwindow * window, double offset_x, double offset_y) {
    onMouseScrollEvent.dispatch(offset_y);
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    onWindowResizeEvent.dispatch(width, height);
}