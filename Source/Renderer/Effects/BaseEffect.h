#pragma once

#include "Core/Base.h"
#include "Core/Gfx/Gfx.h"
#include "Scene/CommonTool.h"
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
	virtual void update(float t)
    {
        Input* input = m_context->getInput();
        Camera* camera = m_context->getCamera();
        if (input->m_mouse_button_down[1])
        {
            input->m_mouse_previou_position = input->m_mouse_position;
        }
        if (input->m_mouse_button_held[1])
        {
            camera->Rotate(input->m_mouse_position - input->m_mouse_previou_position);
            input->m_mouse_previou_position = input->m_mouse_position;
        }
        camera->Move(input->m_mouse_scroll_wheel * 5.0);
    }
	virtual void render() = 0;
	virtual void resize(int width, int height)
	{
		m_width = width;
		m_height = height;
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
