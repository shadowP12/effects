#pragma once
#include "../Base.h"
#include "../../Math/Math.h"
EFFECTS_NAMESPACE_BEGIN

// Camera
class Camera
{
public:
	Camera(glm::vec3 position, float yaw = -90.0f, float pitch = 0.0f);
	~Camera();
	void Move(float wheel);
	void Rotate(glm::vec2 offset);
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix(int width, int height, float fov = 45, float near = 0.1, float far = 1000.0);
private:
	void updateVectors();
private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	float m_yaw;
	float m_pitch;
};

// Input
class Input
{
public:
	Input();
	~Input();
	void update();
public:
	float m_mouse_scroll_wheel;
	bool m_mouse_button_held[3];
	bool m_mouse_button_down[3];
	bool m_mouse_button_up[3];
	glm::vec2 m_mouse_position;
	glm::vec2 m_mouse_previou_position;
};

// Context
class Context
{
public:
	Context();
	~Context();
	void update(float t);
	void setCamera(Camera* camera) { m_camera = camera; }
	void setInput(Input* input) { m_input = input; }
	Camera* getCamera() { return m_camera; }
	Input* getInput() { return m_input; }
	float getTime() { return m_time; }
private:
	Input* m_input;
	Camera* m_camera;
	float m_time;
};

EFFECTS_NAMESPACE_END