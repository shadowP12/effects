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

void destroy_camera(Camera* camera);