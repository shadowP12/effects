#pragma once
#include "../Base.h"
#include "Gfx.h"
#include "../Utility/Module.h"
#include <map>
#include <string>
EFFECTS_NAMESPACE_BEGIN

class GpuProgramDefines : public std::map<std::string, std::string>
{
public:
	GpuProgramDefines& add(const std::string& name, const std::string& val = "") { (*this)[name] = val; return *this; }
	GpuProgramDefines& remove(const std::string& name) { (*this).erase(name); return *this; }
};

class GpuProgram
{
public:
	GpuProgram(std::string vertex_source, std::string frament_source);
	~GpuProgram();
	GLuint getGpuProgram();
	GLuint getGpuProgram(const GpuProgramDefines& defines);
private:
	GLuint createVariation(const GpuProgramDefines& defines);
private:
	std::string m_vertex_source;
	std::string m_frament_source;
	std::map<GpuProgramDefines, GLuint> m_variations;
};

enum class BuiltinProgramType
{
    PBR = 0,
};

class GpuProgramPool : public Module<GpuProgramPool>
{
public:
    GpuProgramPool();
    ~GpuProgramPool();
    GpuProgram* getProgram(const BuiltinProgramType& type);
private:
    std::map<std::string, GpuProgram*> mProgramCache;

};
EFFECTS_NAMESPACE_END