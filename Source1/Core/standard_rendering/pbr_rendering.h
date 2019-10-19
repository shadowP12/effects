#pragma once
#include "../core_renderer.h"
#include "../../utils/gltf_utils.h"

class PBRRenderer : public CoreRenderer
{
public:
	PBRRenderer(int width, int height);
	~PBRRenderer();
	virtual void prepare();
	virtual void update(float t);
	virtual void render();
private:
	GltfScene* m_scene;
	GLuint m_program;	
};
