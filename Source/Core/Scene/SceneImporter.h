#pragma once
#include "../Base.h"
#include "../../Math/Math.h"
#include "../Utility/Module.h"
#include <vector>
#include <map>
#include <string>
EFFECTS_NAMESPACE_BEGIN

class Scene;

class SceneImporter :public Module<SceneImporter>
{
public:
	SceneImporter();
	~SceneImporter();
	Scene* loadScene(std::string file);
private:

};

EFFECTS_NAMESPACE_END