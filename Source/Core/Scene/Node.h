#pragma once
#include "../Base.h"
#include "../../Math/Math.h"
#include <vector>
#include <string>
EFFECTS_NAMESPACE_BEGIN

struct Node
{
	int id;
	int parent;
	std::string name;
	std::vector<int> childrens;
	glm::vec3 translation;
	glm::vec3 scale;
	glm::quat rotation;
};

EFFECTS_NAMESPACE_END