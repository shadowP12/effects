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
	glm::vec3 getPosition() { return m_position; }
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

// 测试阶段:目前只有方向光
struct Light
{
	glm::vec3 direction = glm::vec3(0.0f);
	glm::vec3 position = glm::vec3(0.0f);
	float radius = 1.0f;
	float intensity = 1000.0f;
};

class UISystem;

// Context
class Context
{
public:
	Context();
	~Context();
	void update(float t);
	void setCamera(Camera* camera) { m_camera = camera; }
	void setInput(Input* input) { m_input = input; }
	void setUISystem(UISystem* ui_system) { m_ui_system = ui_system; }
	Camera* getCamera() { return m_camera; }
	Input* getInput() { return m_input; }
	UISystem* getUISystem() { return m_ui_system; }
	float getTime() { return m_time; }
	void drawUI();
private:
	Input* m_input;
	Camera* m_camera;
	UISystem* m_ui_system;
	float m_time;
};

EFFECTS_NAMESPACE_END