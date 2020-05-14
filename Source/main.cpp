#include <iostream>
#include "Core/Utility/FileUtility.h"
#include "Core/Gfx/Gfx.h"
#include "Core/Scene/CommonTool.h"
#include "UI/UISystem.h"
#include "Effects/DebugEffect.h"
#include "Effects/PBREffect.h"
#include "Effects/ShadowEffect.h"
#include "Effects/TerrainEffect.h"
#include "Core/Utility/Flags.h"
#include "Core/Gfx/GpuProgram.h"
#include "Core/Renderer/RenderView.h"
#include "Core/Renderer/Renderer.h"
#include "Importers/GltfImporter.h"
#include "Core/Utility/FileUtility.h"
#include "Core/Utility/Hash.h"
#include "Core/Scene/Scene.h"
#include "Core/Components/CCamera.h"
#define SCREEN_WIDTH 800 
#define SCREEN_HEIGHT 600

//全局变量
et::Camera* g_camera = nullptr;
et::Input* g_input = nullptr;
et::UISystem* g_ui_system = nullptr;
et::Context* g_context = nullptr;
et::TerrainEffect* g_effect = nullptr;
std::shared_ptr<et::SceneObject> gMainCamera;
GLFWwindow* g_window = nullptr;
static float gPitch = 0.0f;
static float gYaw = 0.0f;
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
	g_effect = new et::TerrainEffect(SCREEN_WIDTH, SCREEN_HEIGHT);
	g_effect->setContext(g_context);
	g_effect->prepare();

	// module
	et::Renderer::startUp();
	et::SceneManager::startUp();
	et::GpuProgramPool::startUp();

    gMainCamera = std::make_shared<et::SceneObject>();
    gMainCamera->initialized();
    gMainCamera->addComponent<et::CCamera>();
    //gMainCamera->rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
    //glm::quat rot = fromAxisAngle(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(gYaw));
    //rot = glm::normalize(rot);
    //gMainCamera->setRotation(rot);
    glm::vec3 cameraPos = gMainCamera->getPosition();
    glm::vec3 cameraFront = gMainCamera->getFrontVector();
    cameraPos += cameraFront * 10.0f;
    gMainCamera->setPosition(cameraPos);
}

void release()
{
	delete g_context;
	delete g_input;
	delete g_camera;
	delete g_ui_system;
	et::Renderer::shutDown();
	et::SceneManager::shutDown();
	et::GpuProgramPool::shutDown();
}

void loadResource()
{
}

void update(float t)
{
	g_effect->update(t);
	g_context->update(t);
//    et::Input* input = g_input;
//    et::Camera* camera = g_camera;
//    if (input->m_mouse_button_down[1])
//    {
//        input->m_mouse_previou_position = input->m_mouse_position;
//    }
//    if (input->m_mouse_button_held[1])
//    {
//        glm::vec2 offset = input->m_mouse_position - input->m_mouse_previou_position;
//        //float yaw = gMainCamera->getYaw();
//        //float pitch = gMainCamera->getPitch();
//        gYaw += offset.x * 0.1f;
//        gPitch -= offset.y * 0.1f;
//        glm::vec3 cameraFront = gMainCamera->getFrontVector();
//        glm::vec3 cameraUp = gMainCamera->getUpVector();
//        glm::vec3 cameraRight = gMainCamera->getRightVector();
//        glm::quat yRot = fromAxisAngle(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(gYaw));
//        glm::quat xRot = fromAxisAngle(glm::normalize(cameraRight), glm::radians(gPitch));
//        glm::vec3 front = glm::normalize(getAxisZ(gMainCamera->getWorldMatrix()));
//        printf("new front :  %f  %f  %f\n",front.x,front.y,front.z);
//        glm::quat camRot =  glm::normalize(xRot*yRot);
//        gMainCamera->setRotation(camRot);
//        //gMainCamera->setPitch(gPitch);
//        //gMainCamera->setYaw(gYaw);
//        camera->Rotate(input->m_mouse_position - input->m_mouse_previou_position);
//        input->m_mouse_previou_position = input->m_mouse_position;
//    }
//    glm::vec3 cameraPos = gMainCamera->getPosition();
//    glm::vec3 cameraFront = gMainCamera->getFrontVector();
//    cameraPos += cameraFront * input->m_mouse_scroll_wheel * 5.0f * 0.1f;
//    gMainCamera->setPosition(cameraPos);
//	et::SceneManager::instance().update();
//    g_input->update();
}

void render()
{
    //et::Renderer::instance().render(g_camera);
	g_effect->render();
}

int main()
{
	//初始化glfw
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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
    loadResource();
	while (!glfwWindowShouldClose(g_window))
	{
		update(0.001);
		render();
//        ImGuiIO& io = ImGui::GetIO();
//        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//		g_ui_system->update();
//        bool isWin = true;
//        bool isWin1 = true;
//
//        ImGuiViewport* viewport = ImGui::GetMainViewport();
//        ImGui::SetNextWindowPos(viewport->GetWorkPos());
//        ImGui::SetNextWindowSize(viewport->GetWorkSize());
//        ImGui::SetNextWindowViewport(viewport->ID);
//        ImGui::Begin("Another Window" , &isWin);
//        ImGui::Text("Hello from another window!");
//        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
//        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
//        ImGui::End();
//        ImGui::ShowDemoWindow(&isWin);
//        ImGui::Render();
//        // 将收集完的ui数据绘制到屏幕上
//        glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		g_ui_system->draw();
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