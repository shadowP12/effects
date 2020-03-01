#include "Renderer.h"
#include "Renderable.h"
#include "RenderView.h"
#include "../RenderResources/Mesh.h"
#include "../RenderResources/Material.h"
#include "../Scene/RenderScene.h"
#include "../Scene/CommonTool.h"
EFFECTS_NAMESPACE_BEGIN

Renderer::Renderer()
{}

Renderer::~Renderer()
{}

void Renderer::notifyRenderViewAdded(std::shared_ptr<RenderView> view)
{
    mMainView = view;
}

void Renderer::notifyRenderableAdded(std::shared_ptr<Renderable> renderable)
{
    mRenderables.push_back(renderable);
}

void Renderer::render(Camera* cam)
{
    // seting global defines
    glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (int i = 0; i < mRenderables.size(); ++i)
    {
        std::shared_ptr<Mesh> mesh = mRenderables[i]->getMesh();
        std::shared_ptr<Material> material = mRenderables[i]->getMaterial();
        material->setProgramDefines(mGlobalDefines);
        GpuProgram* program = material->getProgram();
        GLuint programID = program->getGpuProgram(mGlobalDefines);
        glUseProgram(programID);
        glm::mat4 modelMatrix = mRenderables[i]->getTransform();
        glm::mat4 viewMatrix = mMainView->getViewMatrix();//cam->getViewMatrix();
        glm::mat4 projMatrix = cam->getProjectionMatrix(800, 600);//mMainView->getProjMatrix();
        glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programID, "view"), 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programID, "projection"), 1, GL_FALSE, &projMatrix[0][0]);
        // 灯光参数
        glm::vec3 lightPosition = glm::vec3(0.0, 0.0, 0.0);
        glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);
        float lightRadius = 0.5f;
        float lightIntensity = 10.0f;
        glUniform3fv(glGetUniformLocation(programID, "u_lightPos"), 1, &lightPosition[0]);
        glUniform3fv(glGetUniformLocation(programID, "u_lightColor"), 1, &lightColor[0]);
        glUniform1fv(glGetUniformLocation(programID, "u_lightRadius"), 1, &lightRadius);
        glUniform1fv(glGetUniformLocation(programID, "u_lightIntensity"), 1, &lightIntensity);
        // 材质参数
        glm::vec3 albedo = glm::vec3(0.8f, 0.3f, 0.3f);
        glUniform3fv(glGetUniformLocation(programID, "u_albedo"), 1, &albedo[0]);

        // drawing
        glBindVertexArray(mesh->getVertexBufferArray());
        glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

EFFECTS_NAMESPACE_END