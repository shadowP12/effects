#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>

inline glm::vec3 getAxisX(const glm::mat4& mat)
{
	return glm::vec3(mat[0][0], mat[1][0], mat[2][0]);
}

inline glm::vec3 getAxisY(const glm::mat4& mat)
{
	return glm::vec3(mat[0][1], mat[1][1], mat[2][1]);
}

inline glm::vec3 getAxisZ(const glm::mat4& mat)
{
	return glm::vec3(mat[0][2], mat[1][2], mat[2][2]);
}