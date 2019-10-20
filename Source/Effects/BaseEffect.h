#pragma once

#include "../Core/Base.h"
#include "../Core/Gfx/Gfx.h"
#include "../Core/Scene/CommonTool.h"
EFFECTS_NAMESPACE_BEGIN

class BaseEffect
{
public:
	BaseEffect(int width, int height)
	{
		resize(width, height);
	}
	virtual ~BaseEffect()
	{	
	}
	virtual void prepare() = 0;
	virtual void update(float t) = 0;
	virtual void render() = 0;
	virtual void resize(int width, int height)
	{
		m_width = width;
		m_height = height;
		glViewport(0, 0, width, height);
	}

	void setContext(Context* context)
	{
		m_context = context;
	}
protected:
	int m_width;
	int m_height;
	Context* m_context;
};

EFFECTS_NAMESPACE_END
