#pragma once
#include "Core/Base.h"
#include "Core/Utility/Module.h"
#include "Core/Utility/Log.h"
#include "Resources/Resource.h"
#include <vector>
#include <map>
#include <memory>
EFFECTS_NAMESPACE_BEGIN

class ResourceManager : public Module<ResourceManager>
{
public:
    ResourceManager();
    ~ResourceManager();

    const std::map<std::string, std::shared_ptr<Resource>>& getAllRes() { return mResources;}

    std::vector<std::shared_ptr<Resource>> findRes(const std::string& id);

    std::shared_ptr<Resource> getRes(const std::string& uuid);

    void addRes(std::shared_ptr<Resource> res);

    void removeRes(std::shared_ptr<Resource> res);

private:
    std::map<std::string, std::shared_ptr<Resource>> mResources;
};

EFFECTS_NAMESPACE_END