#include <iostream>
#include "Core/Gfx/Gfx.h"
#include "UI/UISystem.h"
#include "Renderer/RenderView.h"
#include "Renderer/Renderer.h"
#include "Importers/GltfImporter.h"
#include "Core/InputSystem/InputSystem.h"
#include "Core/Utility/FileUtility.h"
#include "Core/Utility/Event.h"
#include "Core/Utility/Hash.h"
#include "Scene/Scene.h"
#include "Editor/Editor.h"
#include "Resources/ResourceManager.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#define SCREEN_WIDTH 800 
#define SCREEN_HEIGHT 600

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
    et::ResourceManager::startUp();
	et::Renderer::startUp();
	et::Scene::startUp();
	// editor
    et::Editor::startUp();
	et::Editor::instance().init(glfwGetWin32Window(g_window));
}

void release() {
	et::InputSystem::shutDown();
    et::ResourceManager::shutDown();
	et::Renderer::shutDown();
	et::Scene::shutDown();
	// editor
	et::Editor::shutDown();
}

void update(float dt) {
	et::Scene::instance().update(dt);
	et::Editor::instance().update(dt);
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
	    LOGE("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(g_window);
	glfwSetMouseButtonCallback(g_window, mouse_button_callback);
	glfwSetCursorPosCallback(g_window, cursor_pos_callback);
	glfwSetScrollCallback(g_window, mouse_scroll_callback);
	glfwSetFramebufferSizeCallback(g_window, window_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOGE("Failed to initialize GLAD\n");
		return -1;
	}
	init();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    auto& style = ImGui::GetStyle();
    auto& colors = style.Colors;

    ImGui::StyleColorsDark();

    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Rounding
    style.GrabRounding = 2.3f;
    style.FrameRounding = 2.3f;

    ImGui_ImplGlfw_InitForOpenGL(g_window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    while (!glfwWindowShouldClose(g_window)) {
        glfwPollEvents();
        update(0.001);
        et::Renderer::instance().render();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        et::Editor::instance().render();

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