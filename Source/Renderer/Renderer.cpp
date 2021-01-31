#include "Renderer.h"
#include "Renderable.h"
#include "RenderView.h"
#include "RenderResources/Mesh.h"
#include "RenderResources/Material.h"
EFFECTS_NAMESPACE_BEGIN

Renderer::Renderer() {
    mQuadMesh = std::shared_ptr<Mesh>(genQuadMesh());
}

Renderer::~Renderer() {
    for (int i = 0; i < mViews.size(); ++i) {
        SAFE_DELETE(mViews[i]);
    }
    mViews.clear();

    for (int i = 0; i < mRenderables.size(); ++i) {
        SAFE_DELETE(mRenderables[i]);
    }
    mRenderables.clear();
}

RenderView* Renderer::addRenderView() {
    RenderView* view = new RenderView();
    mViews.push_back(view);
    return view;
}

Renderable* Renderer::addRenderable() {
    Renderable* renderable = new Renderable();
    mRenderables.push_back(renderable);
    return renderable;
}

void Renderer::render()
{

    // output
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


//    // seting global defines
//    glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    for (int i = 0; i < mRenderables.size(); ++i)
//    {
//        std::shared_ptr<Mesh> mesh = mRenderables[i]->getMesh();
//        std::shared_ptr<Material> material = mRenderables[i]->getMaterial();
//        material->setProgramDefines(mGlobalDefines);
//        GpuProgram* program = material->getProgram();
//        GLuint programID = program->getGpuProgram(mGlobalDefines);
//        glUseProgram(programID);
//        glm::mat4 modelMatrix = glm::mat4(1.0f);//mRenderables[i]->getTransform();
//        glm::mat4 viewMatrix = mMainView->getViewMatrix();//cam->getViewMatrix();
//        glm::mat4 projMatrix = cam->getProjectionMatrix(800, 600);//mMainView->getProjMatrix();
//        glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
//        glUniformMatrix4fv(glGetUniformLocation(programID, "view"), 1, GL_FALSE, &viewMatrix[0][0]);
//        glUniformMatrix4fv(glGetUniformLocation(programID, "projection"), 1, GL_FALSE, &projMatrix[0][0]);
//        // �ƹ����
//        glm::vec3 lightPosition = glm::vec3(0.0, 0.0, 0.0);
//        glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);
//        float lightRadius = 0.5f;
//        float lightIntensity = 10.0f;
//        glUniform3fv(glGetUniformLocation(programID, "u_lightPos"), 1, &lightPosition[0]);
//        glUniform3fv(glGetUniformLocation(programID, "u_lightColor"), 1, &lightColor[0]);
//        glUniform1fv(glGetUniformLocation(programID, "u_lightRadius"), 1, &lightRadius);
//        glUniform1fv(glGetUniformLocation(programID, "u_lightIntensity"), 1, &lightIntensity);
//        // ���ʲ���
//        glm::vec3 albedo = glm::vec3(0.8f, 0.3f, 0.3f);
//        glUniform3fv(glGetUniformLocation(programID, "u_albedo"), 1, &albedo[0]);
//
//        // drawing
//        glBindVertexArray(mesh->getVertexBufferArray());
//        glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0);
//    }
}

EFFECTS_NAMESPACE_END