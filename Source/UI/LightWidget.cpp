#include "LightWidget.h"
#include "Scene/CommonTool.h"
EFFECTS_NAMESPACE_BEGIN

LightWidget::LightWidget()
{
}

LightWidget::~LightWidget()
{
}

void LightWidget::draw()
{
	ImGui::Begin("light");

    if(ImGui::TreeNode("Camera"))
    {
        ImGui::SliderFloat("aperture", &mAperture, 0.0f, 3.0f);
        ImGui::SliderFloat("shutterSpeed", &mShutterSpeed, 0.0f, 1.0f);
        ImGui::SliderFloat("sensitivity", &mSensitivity, 0.0f, 200.0f);
        ImGui::TreePop();
    }

    if(ImGui::TreeNode("IBL"))
    {
        ImGui::Checkbox("enable", &enableIBL);
        ImGui::TreePop();
    }

    if(ImGui::TreeNode("Main Lit"))
    {
        ImGui::SliderFloat("dirX", &mMainLitDir.x, -10.0f, 10.0f);
        ImGui::SliderFloat("dirY", &mMainLitDir.y, -10.0f, 10.0f);
        ImGui::SliderFloat("dirZ", &mMainLitDir.z, -10.0f, 10.0f);
        ImGui::ColorEdit4("color", &mMainLitColorIntensity[0], ImGuiColorEditFlags_NoAlpha);
        ImGui::SliderFloat("intensity", &mMainLitColorIntensity.a, 0.0f, 2000.0f);
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