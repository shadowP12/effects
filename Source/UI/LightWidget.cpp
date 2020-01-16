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
	ImGui::SliderFloat("pos_x", &m_light->position.x, -100.0f, 100.0f);
	ImGui::SliderFloat("pos_y", &m_light->position.y, -100.0f, 100.0f);
	ImGui::SliderFloat("pos_z", &m_light->position.z, -100.0f, 100.0f);
    ImGui::SliderFloat("radius", &m_light->radius, -100.0f, 100.0f);
	ImGui::End();
}

EFFECTS_NAMESPACE_END