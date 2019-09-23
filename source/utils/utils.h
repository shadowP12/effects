#pragma once
#include "base.h"

//Camera
struct Camera
{
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	float yaw;
	float pitch;
};

Camera* create_camera(glm::vec3 position, float yaw = -90.0f, float pitch = 0.0f);

void camera_move(Camera* camera, float wheel);

void camera_rotate(Camera* camera, glm::vec2 offset);

glm::mat4 get_camera_view_matrix(Camera* camera);

glm::mat4 get_camera_projection_matrix(Camera* camera, int width, int height, float fov = 45, float near = 0.1, float far = 100.0);

void destroy_camera(Camera* camera);

//Input
struct Input
{
	float mouse_scroll_wheel;
	bool mouse_button_held[3];
	bool mouse_button_down[3];
	bool mouse_button_up[3];
	glm::vec2 mouse_position;
	glm::vec2 mouse_previou_position;
};

Input* create_input();

void uptate_input(Input* input);

void destroy_input(Input* input);

