#pragma once
#include "../Core/Base.h"
#include "../Core/Gfx/Gfx.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_opengl3.h"
#include <vector>
EFFECTS_NAMESPACE_BEGIN

class UIWidget
{
public:
	UIWidget(){}
	~UIWidget(){}
	virtual void draw() = 0;
};

class UISystem
{
public:
	UISystem(GLFWwindow* window);
	~UISystem();
	void update();
	void draw();
private:
	std::vector<UIWidget> m_widgets;
	
};

EFFECTS_NAMESPACE_END