#pragma once
#include "../Core/Base.h"
#include <string>
#include <map>
#include <memory>
#include <vector>

EFFECTS_NAMESPACE_BEGIN

class Scene;
class Mesh;
class Node;

struct GltfScene
{
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Mesh>> meshs;
    std::map<std::shared_ptr<Node>, std::shared_ptr<Mesh>> meshHelper;
};

class GltfImporter
{
public:
    GltfImporter();
    ~GltfImporter();
    void load(std::string filePath, GltfScene* scene);
private:

};

EFFECTS_NAMESPACE_END