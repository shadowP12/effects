#include "Geometry.h"
EFFECTS_NAMESPACE_BEGIN

float intersect(const Ray& ray, const Plane& plane)
{
	float denom = glm::dot(plane.normal, ray.dir);
	if (denom > 1e-6) 
	{
		glm::vec3 difference = plane.center - ray.orig;
		float t = glm::dot(plane.normal, difference) / denom;
		if (t >= 0)
			return t;
		return -1;
	}
	return -1;
}

EFFECTS_NAMESPACE_END