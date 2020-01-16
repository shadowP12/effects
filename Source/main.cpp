#include <iostream>
#include "Core/Utility/FileUtility.h"
#include "Core/Gfx/Gfx.h"
#include "Core/Scene/CommonTool.h"
#include "UI/UISystem.h"
#include "Effects/DebugEffect.h"
#include "Effects/PBREffect.h"
#include "Effects/ShadowEffect.h"
#define SCREEN_WIDTH 800 
#define SCREEN_HEIGHT 600

//全局变量
et::Camera* g_camera = nullptr;
et::Input* g_input = nullptr;
et::UISystem* g_ui_system = nullptr;
et::Context* g_context = nullptr;
et::PBREffect* g_effect = nullptr;
GLFWwindow* g_window = nullptr;

//窗口回调函数
void cursor_pos_callback(GLFWwindow * window, double pos_x, double pos_y);
void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);
void mouse_scroll_callback(GLFWwindow * window, double offset_x, double offset_y);
void window_size_callback(GLFWwindow* window, int width, int height);

void init()
{
	g_camera = new et::Camera(glm::vec3(0.0, 0.0, 5.0));
	g_input = new et::Input();
	g_ui_system = new et::UISystem(g_window);
	g_context = new et::Context();
	g_context->setInput(g_input);
	g_context->setCamera(g_camera);
	g_context->setUISystem(g_ui_system);

	// effect
	g_effect = new et::PBREffect(SCREEN_WIDTH, SCREEN_HEIGHT);
	g_effect->setContext(g_context);
	g_effect->prepare();
}

void release()
{
	delete g_context;
	delete g_input;
	delete g_camera;
	delete g_ui_system;
}

void update(float t)
{
	g_effect->update(t);
	g_context->update(t);
}

void render()
{
	g_effect->render();
}

int main()
{
	//初始化glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	//创建带窗口的gl环境
	g_window = glfwCreateWindow(800, 600, "effects", NULL, NULL);
	if (g_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//绑定当前gl环境
	glfwMakeContextCurrent(g_window);
	glfwSetMouseButtonCallback(g_window, mouse_button_callback);
	glfwSetCursorPosCallback(g_window, cursor_pos_callback);
	glfwSetScrollCallback(g_window, mouse_scroll_callback);
	glfwSetFramebufferSizeCallback(g_window, window_size_callback);
	//初始化gl函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	init();
	while (!glfwWindowShouldClose(g_window))
	{
		update(0.001);
		render();
		g_context->drawUI();
		glfwSwapBuffers(g_window);
		glfwPollEvents();
	}
	glfwDestroyWindow(g_window);
	glfwTerminate();
	release();
	return 0;
}

void cursor_pos_callback(GLFWwindow * window, double pos_x, double pos_y)
{
	g_input->m_mouse_position = glm::vec2(pos_x, pos_y);
}

void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		g_input->m_mouse_button_down[button] = true;
		g_input->m_mouse_button_held[button] = true;
		break;
	case GLFW_RELEASE:
		g_input->m_mouse_button_up[button] = true;
		g_input->m_mouse_button_held[button] = false;
		break;
	default:
		break;
	}
}

void mouse_scroll_callback(GLFWwindow * window, double offset_x, double offset_y)
{
	g_input->m_mouse_scroll_wheel = offset_y;
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	// resize
	g_effect->resize(width, height);
}