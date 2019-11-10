#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>

inline glm::vec3 getAxisX(const glm::mat4& mat)
{
	return glm::vec3(mat[0][0], mat[0][1], mat[0][2]);
}

inline glm::vec3 getAxisY(const glm::mat4& mat)
{
	return glm::vec3(mat[1][0], mat[1][1], mat[1][2]);
}

inline glm::vec3 getAxisZ(const glm::mat4& mat)
{
	return glm::vec3(mat[2][0], mat[2][1], mat[2][2]);
}

inline glm::vec3 getTranslate(const glm::mat4& mat)
{
	return glm::vec3(mat[3][0], mat[3][1], mat[3][2]);
}

// screen to world space
inline glm::vec3 unProject(const glm::vec4& viewPort, const glm::vec2& point, float depth, const glm::mat4& viewProjectionMatrix)
{
	glm::mat4 inverseViewProjectionMatrix = glm::inverse(viewProjectionMatrix);
	glm::vec4 screen = glm::vec4((point.x - viewPort.x) / viewPort.z, ((viewPort.w - point.y) - viewPort.y) / viewPort.w, depth, 1.0f);

	// to[-1 , 1]
	screen.x = screen.x * 2.0f - 1.0f;
	screen.y = screen.y * 2.0f - 1.0f;
	screen.z = screen.z * 2.0f - 1.0f;

	glm::vec4 worldPoint = inverseViewProjectionMatrix * screen;
	worldPoint.x = worldPoint.x / worldPoint.w;
	worldPoint.y = worldPoint.y / worldPoint.w;
	worldPoint.z = worldPoint.z / worldPoint.w;

	glm::vec3 out = glm::vec3(worldPoint.x, worldPoint.y, worldPoint.z);
	return out;
}

inline void printMatrixInfo(const glm::mat4& mat)
{
	printf("%f   %f   %f   %f\n", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
	printf("%f   %f   %f   %f\n", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
	printf("%f   %f   %f   %f\n", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
	printf("%f   %f   %f   %f\n", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}