#include "CommonTool.h"
#include "../Utility/Log.h"
EFFECTS_NAMESPACE_BEGIN

Camera::Camera(glm::vec3 position, float yaw, float pitch)
{
	m_position = position;
	m_front = glm::vec3(0, 0, -1);
	m_up = glm::vec3(0, 1, 0);
	m_yaw = yaw;
	m_pitch = pitch;
}
Camera::~Camera()
{
}
void Camera::Move(float wheel)
{
	m_position += m_front * wheel * 0.1f;
}
void Camera::Rotate(glm::vec2 offset)
{
	m_yaw += offset.x * 0.1f;
	m_pitch += -offset.y * 0.1f;
	updateVectors();
}
glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}
glm::mat4 Camera::getProjectionMatrix(int width, int height, float fov, float near, float far)
{
	float ratio = width / float(height);
	return glm::perspective(glm::radians(fov), ratio, near, far);
}

void Camera::updateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);
	m_right = glm::normalize(glm::cross(m_front, glm::vec3(0, 1, 0)));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

Input::Input()
{
}

Input::~Input()
{
}

void Input::update()
{
	memset(m_mouse_button_up, 0, sizeof(m_mouse_button_up));
	memset(m_mouse_button_down, 0, sizeof(m_mouse_button_down));
	m_mouse_scroll_wheel = 0.0;
}

Context::Context()
{
}

Context::~Context()
{
}


void Context::update(float t)
{
	m_time += t;
	m_input->update();
}
EFFECTS_NAMESPACE_END