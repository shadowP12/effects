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

	et::Renderer::startUp();
	et::Scene::startUp();
	et::Editor::startUp();
	et::Editor::instance().init();
}

void release() {
	et::InputSystem::shutDown();
	et::Renderer::shutDown();
	et::Scene::shutDown();
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