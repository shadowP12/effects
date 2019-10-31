#include "LightWidget.h"
#include "../Core/Scene/CommonTool.h"
EFFECTS_NAMESPACE_BEGIN

LightWidget::LightWidget(Light* light)
	:m_light(light)
{
}

LightWidget::~LightWidget()
{
}

void LightWidget::draw()
{
	ImGui::Begin("light");
	ImGui::SliderFloat("dir_x", &m_light->direction.x, -1.0f, 1.0f);
	ImGui::SliderFloat("dir_y", &m_light->direction.y, -1.0f, 1.0f);
	ImGui::SliderFloat("dir_z", &m_light->direction.z, -1.0f, 1.0f);
	ImGui::End();
}

EFFECTS_NAMESPACE_END