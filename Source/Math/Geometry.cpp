#include "Geometry.h"
EFFECTS_NAMESPACE_BEGIN

Ray pickRay(const glm::vec4& viewport, const glm::vec2& point, const glm::mat4& view_matrix, const glm::mat4& projection_matrix)
{
	glm::mat4 view_projection_matrix = projection_matrix * view_matrix;
	glm::vec3 near_point = unProject(viewport, point, 0.0f, view_projection_matrix);
	glm::vec3 far_point = unProject(viewport, point, 1.0f, view_projection_matrix);
	glm::vec3 dir = glm::normalize(far_point - near_point);
	glm::vec3 orig = getTranslate(view_matrix) * -1.0f;
	Ray ray;
	ray.dir = dir;
	ray.orig = orig;
	return ray;
}

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