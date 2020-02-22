#pragma once
#include "../Core/Base.h"
#include <string>
EFFECTS_NAMESPACE_BEGIN

class Scene;

class GltfImporter
{
public:
    GltfImporter();
    ~GltfImporter();
    void load(std::string filePath, Scene* scene);
private:

};

EFFECTS_NAMESPACE_END