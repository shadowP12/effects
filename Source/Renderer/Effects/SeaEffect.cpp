#include "Renderer/Effects/SeaEffect.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderView.h"
#include "Renderer/Renderable.h"
#include "Renderer/SeaRenderable.h"
#include "Renderer/RenderLight.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "Resources/Texture.h"
#include "Core/Gfx/GfxResources.h"
#include "Core/Utility/Hash.h"
#include "Core/Utility/FileUtility.h"
EFFECTS_NAMESPACE_BEGIN

static std::string gVertSource;
static std::string gFragSource;

SeaEffect::SeaEffect()
    :BaseEffect() {
    readFileData("./BuiltinResources/Shaders/SeaEffect.vert", gVertSource);
    readFileData("./BuiltinResources/Shaders/SeaEffect.frag", gFragSource);
}

SeaEffect::~SeaEffect() {
    for (auto& program : mProgramCache) {
        destroyGfxProgram(program.second);
    }
}

void SeaEffect::render(std::vector<RenderView*> views, std::vector<Renderable*> renderables) {
    for (int i = 0; i < views.size(); ++i) {
        glm::vec4 viewport = views[i]->getViewPort();
        glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
        bindGfxFramebuffer(views[i]->getRenderTarget());
        glEnable(GL_DEPTH_TEST);
        // render
        glm::mat4 viewMatrix = views[i]->getViewMatrix();
        glm::mat4 projMatrix = views[i]->getProjMatrix();
        for (int j = 0; j < renderables.size(); ++j) {
            SeaRenderable* meshRenderable = static_cast<SeaRenderable*>(renderables[j]);
            uint32_t bits = 0;
            if (Renderer::instance().getDirectLights().size() > 0) {
                bits |= (uint32_t)Bit::USE_DIRECT_LIGHT;
            }

            GfxProgram* program = getProgram(bits);
            std::shared_ptr<Mesh> mesh = meshRenderable->getMesh();
            glm::mat4 modelMatrix = meshRenderable->getTransform();
            setGfxProgramMat4(program, "u_model", &modelMatrix[0][0]);
            setGfxProgramMat4(program, "u_view", &viewMatrix[0][0]);
            setGfxProgramMat4(program, "u_projection", &projMatrix[0][0]);
            setGfxProgramFloat3(program, "u_viewPos", glm::value_ptr(views[i]->getPosition()));
            if((bits & (uint32_t)Bit::USE_DIRECT_LIGHT) != 0) {
                setGfxProgramFloat4(program, "u_mainLitDirection", &Renderer::instance().getDirectLights()[0]->getDirection()[0]);
                setGfxProgramFloat4(program, "u_mainLitColorAndIntensity", &Renderer::instance().getDirectLights()[0]->getLightColorAndIntensity()[0]);
            }
            setGfxProgramSampler(program, "u_heightMap", meshRenderable->getHeightMap());
            setGfxProgramSampler(program, "u_normalMap", meshRenderable->getNormalMap());
            bindGfxProgram(program);
            mesh->draw(GL_TRIANGLES);
            unbindGfxProgram(program);
        }
        unbindGfxFramebuffer(views[i]->getRenderTarget());
        glDisable(GL_DEPTH_TEST);
    }
}

GfxProgram* SeaEffect::getProgram(uint32_t bits) {
    auto it = mProgramCache.find(bits);
    if (it == mProgramCache.end()) {
        std::string defines = "";
        if((bits & (uint32_t)Bit::USE_DIRECT_LIGHT) != 0) {
            defines += "#define USE_DIRECT_LIGHT \n";
        }
        GfxProgramDesc programDesc;
        programDesc.vertSource = gVertSource;
        programDesc.fragSource = gFragSource;
        programDesc.define = defines;
        mProgramCache[bits] = createGfxProgram(programDesc);
        return mProgramCache[bits];
    }
    return mProgramCache[bits];
}

EFFECTS_NAMESPACE_END