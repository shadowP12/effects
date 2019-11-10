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

float intersect(const Ray& ray, const Plane& plane);

EFFECTS_NAMESPACE_END