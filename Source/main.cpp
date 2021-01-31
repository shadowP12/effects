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
#include "Scene/Components/CCamera.h"
#define SCREEN_WIDTH 800 
#define SCREEN_HEIGHT 600

et::SceneNode* gMainCamera;
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
    onMousePositionEvent.bind(CALLBACK_2(et::InputSystem::onMousePosition, et::InputSystem::instance()));
    onMouseButtonEvent.bind(CALLBACK_2(et::InputSystem::onMouseButton, et::InputSystem::instance()));
    onMouseScrollEvent.bind(CALLBACK_1(et::InputSystem::onMouseScroll, et::InputSystem::instance()));
    onFrameFinishEvent.bind(CALLBACK_0(et::InputSystem::reset, et::InputSystem::instance()));

	et::Renderer::startUp();
	et::Scene::startUp();

//    gMainCamera = ();
//    gMainCamera->initialized();
//    gMainCamera->addComponent<et::CCamera>();
    //gMainCamera->rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
    //glm::quat rot = fromAxisAngle(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(gYaw));
    //rot = glm::normalize(rot);
    //gMainCamera->setRotation(rot);
//    glm::vec3 cameraPos = gMainCamera->getPosition();
//    glm::vec3 cameraFront = gMainCamera->getFrontVector();
//    cameraPos += cameraFront * 10.0f;
//    gMainCamera->setPosition(cameraPos);
}

void release()
{
	et::InputSystem::shutDown();
	et::Renderer::shutDown();
	et::Scene::shutDown();
}

void update(float t)
{
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
    //g_input->update();
}

int main()
{
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	g_window = glfwCreateWindow(800, 600, "effects", NULL, NULL);
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

	while (!glfwWindowShouldClose(g_window)) {
		update(0.001);
        et::Renderer::instance().render();
		glfwSwapBuffers(g_window);
		glfwPollEvents();
		onFrameFinishEvent.dispatch();
	}
	glfwDestroyWindow(g_window);
	glfwTerminate();
	release();
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

void mouse_scroll_callback(GLFWwindow * window, double offset_x, double offset_y)
{
    onMouseScrollEvent.dispatch(offset_y);
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    onWindowResizeEvent.dispatch(width, height);
}