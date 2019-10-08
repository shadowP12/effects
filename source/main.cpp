#include <iostream>
#include "utils/gl_inc.h"
#include "core/effects_define.h"

//全局变量
Camera* _camera = nullptr;
Input* _input = nullptr;
Device* _device = nullptr;
float _time = 0.0;
float _timer = 0.0;
OceanRenderer* _ocean_renderer = nullptr;

//窗口回调函数
void cursor_pos_callback(GLFWwindow * window, double pos_x, double pos_y);
void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);
void mouse_scroll_callback(GLFWwindow * window, double offset_x, double offset_y);
void window_size_callback(GLFWwindow* window, int width, int height);

void init()
{
	_camera = create_camera(glm::vec3(0,0,0));
	_input = create_input();
	_device = create_device();
	set_device_input(_device, _input);
	set_device_camera(_device, _camera);
	_ocean_renderer = new OceanRenderer(800, 600);
	_ocean_renderer->set_device(_device);
	_ocean_renderer->prepare();
}

void release()
{
	delete _ocean_renderer;
	destroy_input(_input);
	destroy_camera(_camera);
	destroy_device(_device);
}

void update(float t)
{
	if (_input->mouse_button_down[1])
	{
		_input->mouse_previou_position = _input->mouse_position;
	}
	if (_input->mouse_button_held[1])
	{
		//LOGI("%f %f , %f %f\n",_input->mouse_position.x, _input->mouse_position.y, _input->mouse_previou_position.x, _input->mouse_previou_position.y);
		camera_rotate(_camera, _input->mouse_position - _input->mouse_previou_position);
		_input->mouse_previou_position = _input->mouse_position;
	}
	camera_move(_camera, _input->mouse_scroll_wheel * 5.0);
	uptate_input(_input);
	_time += t;
	_timer += t;
	if (_timer > 0.15)
	{
		_ocean_renderer->update(_time);
		_timer = 0.0;
	}
}

void render()
{
	_ocean_renderer->render();
}

int main()
{
	//初始化glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//创建带窗口的gl环境
	GLFWwindow* window = glfwCreateWindow(800, 600, "effects", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//绑定创建好的gl环境
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetFramebufferSizeCallback(window, window_size_callback);
	//初始化gl函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	init();
	while (!glfwWindowShouldClose(window))
	{
		update(0.001);
		render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	release();
	return 0;
}

void cursor_pos_callback(GLFWwindow * window, double pos_x, double pos_y)
{
	_input->mouse_position = glm::vec2(pos_x, pos_y);
}

void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		_input->mouse_button_down[button] = true;
		_input->mouse_button_held[button] = true;
		break;
	case GLFW_RELEASE:
		_input->mouse_button_up[button] = true;
		_input->mouse_button_held[button] = false;
		break;
	default:
		break;
	}
}

void mouse_scroll_callback(GLFWwindow * window, double offset_x, double offset_y)
{
	_input->mouse_scroll_wheel = offset_y;
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	_ocean_renderer->resize(width, height);
}