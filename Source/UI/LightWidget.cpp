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

    if(ImGui::TreeNode("Main Lit"))
    {
        ImGui::SliderFloat("dirX", &m_light->mainLitDir.x, -10.0f, 10.0f);
        ImGui::SliderFloat("dirY", &m_light->mainLitDir.y, -10.0f, 10.0f);
        ImGui::SliderFloat("dirZ", &m_light->mainLitDir.z, -10.0f, 10.0f);
        ImGui::ColorEdit4("color", &m_light->mainLitColorIntensity[0], ImGuiColorEditFlags_NoAlpha);
        ImGui::SliderFloat("intensity", &m_light->mainLitColorIntensity.a, 0.0f, 2000.0f);
        ImGui::TreePop();
    }
//    if(ImGui::TreeNode("Point Lit 0"))
//    {
//        ImGui::SliderFloat("x", &m_light->position.x, -100.0f, 100.0f);
//        ImGui::SliderFloat("y", &m_light->position.y, -100.0f, 100.0f);
//        ImGui::SliderFloat("z", &m_light->position.z, -100.0f, 100.0f);
//        ImGui::SliderFloat("radius", &m_light->radius, 0.0f, 100.0f);
//        ImGui::SliderFloat("intensity", &m_light->intensity, 0.0f, 2000.0f);
//        ImGui::TreePop();
//        //ImGui::Separator();
//    }
//
//    if(ImGui::TreeNode("Point Lit 1"))
//    {
//        ImGui::SliderFloat("x", &m_light->position.x, -100.0f, 100.0f);
//        ImGui::SliderFloat("y", &m_light->position.y, -100.0f, 100.0f);
//        ImGui::SliderFloat("z", &m_light->position.z, -100.0f, 100.0f);
//        ImGui::SliderFloat("radius", &m_light->radius, 0.0f, 100.0f);
//        ImGui::SliderFloat("intensity", &m_light->intensity, 0.0f, 2000.0f);
//        ImGui::TreePop();
//        //ImGui::Separator();
//    }

	ImGui::End();
}

EFFECTS_NAMESPACE_END