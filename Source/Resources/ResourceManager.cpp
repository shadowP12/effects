#include "Resources/ResourceManager.h"

EFFECTS_NAMESPACE_BEGIN

ResourceManager::ResourceManager() {
}

ResourceManager::~ResourceManager() {
}

std::vector<std::shared_ptr<Resource>> ResourceManager::findRes(const std::string& id) {
    std::vector<std::shared_ptr<Resource>> findResults;
    for (auto& var : mResources) {
        if (var.second->getId() == id) {
            findResults.push_back(var.second);
        }
    }
    return findResults;
}

std::shared_ptr<Resource> ResourceManager::getRes(const std::string& uuid) {
    std::shared_ptr<Resource> res;
    std::map<std::string, std::shared_ptr<Resource>>::iterator it = mResources.find(uuid);
    if (it != mResources.end()) {
        res = it->second;
    }
    return res;
}

void ResourceManager::addRes(std::shared_ptr<Resource> res) {
    std::pair<std::map<std::string, std::shared_ptr<Resource>>::iterator, bool> result;
    result = mResources.insert(std::map<std::string, std::shared_ptr<Resource>>::value_type(res->mUUID, res));

    if (!result.second) {
        LOGW("Res has already exist!\n");
    }
}

void ResourceManager::removeRes(std::shared_ptr<Resource> res) {
    std::map<std::string, std::shared_ptr<Resource>>::iterator it = mResources.find(res->mUUID);
    if (it != mResources.end()) {
        mResources.erase(it);
    }
}

EFFECTS_NAMESPACE_END