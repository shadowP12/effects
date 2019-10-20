#pragma once
#include "../Base.h"
#include <string>
EFFECTS_NAMESPACE_BEGIN

std::string getCurrentPath();

bool readFileData(std::string& file_path, std::string& out_data);

EFFECTS_NAMESPACE_END