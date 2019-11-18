#pragma once	
#include "../Core/Base.h"	
#include "Math.h"	

EFFECTS_NAMESPACE_BEGIN

struct Ray
{
	glm::vec3 pointAt(float t)
	{
		return orig + dir * t;
	}

	static glm::vec3 pointAt(const Ray& ray, float t)
	{
		return ray.orig + ray.dir * t;
	}

	glm::vec3 orig;
	glm::vec3 dir;
};

struct Plane
{
	glm::vec3 normal;
	glm::vec3 center;
};

struct Triangle
{
	glm::vec3 v0, v1, v2;
};

void printRayInfo(const Ray& ray);

Ray pickRay(const glm::vec4& viewport, const glm::vec2& point, const glm::mat4& view_matrix, const glm::mat4& projection_matrix);

float intersect(const Ray& ray, const Plane& plane);

bool intersect(const Ray& ray, const Triangle& triangle, float& t, glm::vec2 uv);


EFFECTS_NAMESPACE_END