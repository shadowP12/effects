#include "Renderer/Effects/DefaultEffect.h"
#include "Renderer/RenderView.h"
#include "Renderer/Renderable.h"
#include "Renderer/MeshRenderable.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "Core/Gfx/GfxResources.h"
#include "Core/Utility/Hash.h"
#include "Core/Utility/FileUtility.h"
EFFECTS_NAMESPACE_BEGIN

static std::string gVertSource;
static std::string gFragSource;

DefaultEffect::DefaultEffect()
    :BaseEffect() {
    readFileData("./BuiltinResources/Shaders/DefaultEffect.vert", gVertSource);
    readFileData("./BuiltinResources/Shaders/DefaultEffect.frag", gFragSource);
}

DefaultEffect::~DefaultEffect() {
    for (auto& program : mProgramCache) {
        destroyGfxProgram(program.second);
    }
}

void DefaultEffect::render(std::vector<RenderView*> views, std::vector<Renderable*> renderables) {
    for (int i = 0; i < views.size(); ++i) {
        glm::vec4 viewport = views[i]->getViewPort();
        glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
        bindGfxFramebuffer(views[i]->getRenderTarget());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        // render
        glm::mat4 viewMatrix = views[i]->getViewMatrix();
        glm::mat4 projMatrix = views[i]->getProjMatrix();
        for (int j = 0; j < renderables.size(); ++j) {
            MeshRenderable* meshRenderable = static_cast<MeshRenderable*>(renderables[j]);
            GfxProgram* program = getProgram(meshRenderable);
            std::shared_ptr<Mesh> mesh = meshRenderable->getMesh();
            glm::mat4 modelMatrix = meshRenderable->getTransform();
            setGfxProgramMat4(program, "u_model", &modelMatrix[0][0]);
            setGfxProgramMat4(program, "u_view", &viewMatrix[0][0]);
            setGfxProgramMat4(program, "u_projection", &projMatrix[0][0]);
            bindGfxProgram(program);
            mesh->draw(GL_TRIANGLES);
            unbindGfxProgram(program);
        }
        unbindGfxFramebuffer(views[i]->getRenderTarget());
        glDisable(GL_DEPTH_TEST);
    }
}

GfxProgram* DefaultEffect::getProgram(MeshRenderable* renderable) {
    size_t hash = 0;
    Hash(hash, renderable->getMesh()->getLayout());

    auto it = mProgramCache.find(hash);
    if (it == mProgramCache.end()) {
        GfxProgramDesc programDesc;
        programDesc.vertSource = gVertSource;
        programDesc.fragSource = gFragSource;
        programDesc.define = "";
        mProgramCache[hash] = createGfxProgram(programDesc);
        return mProgramCache[hash];
    }
    return mProgramCache[hash];
}

EFFECTS_NAMESPACE_END