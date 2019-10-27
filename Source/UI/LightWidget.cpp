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
	ImGui::Begin("test");
	ImGui::Text("light test");
	ImGui::End();
}

EFFECTS_NAMESPACE_END