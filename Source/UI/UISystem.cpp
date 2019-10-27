#include "UISystem.h"

EFFECTS_NAMESPACE_BEGIN

UISystem::UISystem(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");
}

UISystem::~UISystem()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UISystem::addWidget(UIWidget* widget)
{
	m_widgets.push_back(widget);
}

void UISystem::update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UISystem::draw()
{
	for (int i = 0; i < m_widgets.size(); i++)
	{
		m_widgets[i]->draw();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

EFFECTS_NAMESPACE_END