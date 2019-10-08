#pragma once
#include "../../utils/base.h"
#include "../../utils/gl_inc.h"
#include "../../utils/gl_utils.h"
#include "../../utils/utils.h"

class CoreRenderer
{
public:
	CoreRenderer(int width, int height)
	{
		resize(width, height);
	}
	virtual ~CoreRenderer()
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
	void set_device(Device* device)
	{
		m_device = device;
	}
protected:
	int m_width;
	int m_height;
	Device* m_device;
};
