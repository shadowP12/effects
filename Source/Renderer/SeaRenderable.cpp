#include "Renderer/SeaRenderable.h"
#include "Renderer/Renderer.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "Scene/Components/CSea.h"
#include "Core/Utility/FileUtility.h"
EFFECTS_NAMESPACE_BEGIN

SeaRenderable::SeaRenderable(CSea* csea){
    mCSea = csea;
    mType = RenderableType::SEA;
    std::string vs, fs;
    GfxProgramDesc programDesc;
    readFileData("./BuiltinResources/Shaders/Sea/SeaOctave.vert", vs);
    readFileData("./BuiltinResources/Shaders/Sea/SeaOctave.frag", fs);
    programDesc.vertSource = vs;
    programDesc.fragSource = fs;
    programDesc.define = "";
    mSeaOctave = createGfxProgram(programDesc);

    readFileData("./BuiltinResources/Shaders/Sea/SeaNormal.vert", vs);
    readFileData("./BuiltinResources/Shaders/Sea/SeaNormal.frag", fs);
    programDesc.vertSource = vs;
    programDesc.fragSource = fs;
    programDesc.define = "";
    mSeaNormal = createGfxProgram(programDesc);
}

SeaRenderable::~SeaRenderable() {
    destroyGfxFramebuffer(mProcessFB);
    destroyGfxTexture(mHeightMap);
    destroyGfxSampler(mSampler);
    destroyGfxProgram(mSeaOctave);
    destroyGfxProgram(mSeaNormal);
}

void SeaRenderable::initialize() {
}

void SeaRenderable::setMesh(std::shared_ptr<Mesh> mesh) {
    mMesh = mesh;
}

void SeaRenderable::setMaterial(std::shared_ptr<Material> material) {
    mMaterial = material;
}

void SeaRenderable::setTransform(const glm::mat4 &mat) {
    mWorldMatrix = mat;
}

std::shared_ptr<Mesh> SeaRenderable::getMesh() {
    return mMesh;
}

std::shared_ptr<Material> SeaRenderable::getMaterial() {
    return mMaterial;
}

glm::mat4 SeaRenderable::getTransform() {
    return mWorldMatrix;
}

void SeaRenderable::updateSeaMap() {
    if (mShouldReset) {
        GfxSamplerDesc samplerDesc;
        mSampler = createGfxSampler(samplerDesc);

        GfxTextureDesc texDesc;
        texDesc.width = mCSea->mSeaX;
        texDesc.height = mCSea->mSeaY;
        texDesc.depth = 1;
        texDesc.arraySize = 1;
        texDesc.format = GL_RGB;
        texDesc.internalFormat = GL_RGB16F;
        texDesc.componentType = GL_FLOAT;
        mHeightMap = createGfxTexture(texDesc);
        writeGfxTextureData(mHeightMap, nullptr);
        setGfxTextureSampler(mHeightMap, mSampler);

        texDesc.format = GL_RGB;
        texDesc.internalFormat = GL_RGB16F;
        mNormalMap = createGfxTexture(texDesc);
        writeGfxTextureData(mNormalMap, nullptr);
        setGfxTextureSampler(mNormalMap, mSampler);

        GfxFramebufferDesc fbDesc;
        mProcessFB = createGfxFramebuffer();
        mShouldReset = false;
    }

    attachGfxFramebufferTexture(mProcessFB, 0, mHeightMap);
    glViewport(0, 0, mCSea->mSeaX, mCSea->mSeaY);
    bindGfxFramebuffer(mProcessFB);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setGfxProgramFloat(mSeaOctave, "u_seaTime", mCSea->mSeaTime * 10.0);
    bindGfxProgram(mSeaOctave);
    Renderer::instance().getQuadMesh()->draw(GL_TRIANGLES);
    unbindGfxProgram(mSeaOctave);
    unbindGfxFramebuffer(mProcessFB);

    attachGfxFramebufferTexture(mProcessFB, 0, mNormalMap);
    glViewport(0, 0, mCSea->mSeaX, mCSea->mSeaY);
    bindGfxFramebuffer(mProcessFB);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setGfxProgramFloat2(mSeaNormal, "u_texelSize", glm::value_ptr(glm::vec2(1.0 / mCSea->mSeaX, 1.0 / mCSea->mSeaY)));
    setGfxProgramSampler(mSeaNormal, "u_heightMap", mHeightMap);
    bindGfxProgram(mSeaNormal);
    Renderer::instance().getQuadMesh()->draw(GL_TRIANGLES);
    unbindGfxProgram(mSeaNormal);
    unbindGfxFramebuffer(mProcessFB);
}

EFFECTS_NAMESPACE_END
