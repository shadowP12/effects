#include "utils.h"

//Camera
void update_camera_vectors(Camera* camera)
{
	glm::vec3 front;
	front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
	front.y = sin(glm::radians(camera->pitch));
	front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
	camera->front = glm::normalize(front);
	camera->right = glm::normalize(glm::cross(camera->front, glm::vec3(0, 1, 0)));
	camera->up = glm::normalize(glm::cross(camera->right, camera->front));
}

Camera* create_camera(glm::vec3 position, float yaw, float pitch)
{
	Camera* camera = new Camera;
	camera->position = position;
	camera->front = glm::vec3(0, 0, -1);
	camera->up = glm::vec3(0, 1, 0);
	camera->yaw = yaw;
	camera->pitch = pitch;
	return camera;
}

void camera_move(Camera* camera, float wheel)
{
	camera->position += camera->front * wheel * 0.1f;
}

void camera_rotate(Camera* camera, glm::vec2 offset)
{
	camera->yaw += offset.x * 0.1f;
	camera->pitch += -offset.y * 0.1f;
	update_camera_vectors(camera);
}

glm::mat4 get_camera_view_matrix(Camera* camera)
{
	return glm::lookAt(camera->position, camera->position + camera->front, camera->up);
}

glm::mat4 get_camera_projection_matrix(Camera* camera, int width, int height, float fov, float near, float far)
{
	float ratio = width / float(height);
	return glm::perspective(glm::radians(fov), ratio, near, far);
}

void destroy_camera(Camera* camera)
{
	delete camera;
}

Input* create_input()
{
	Input* input = new Input();
	return input;
}

void uptate_input(Input* input)
{
	memset(input->mouse_button_up, 0, sizeof(input->mouse_button_up));
	memset(input->mouse_button_down, 0, sizeof(input->mouse_button_down));
	input->mouse_scroll_wheel = 0.0;
}

void destroy_input(Input* input)
{
	delete input;
}

Device* create_device()
{
	Device* device = new Device();
	return device;
}

void set_device_input(Device* device, Input* input)
{
	device->input = input;
}

void set_device_camera(Device* device, Camera* camera)
{
	device->camera = camera;
}

void destroy_device(Device* device)
{
	delete device;
}