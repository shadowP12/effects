#include "GltfImporter.h"
#include "Scene/Scene.h"
#include "Datas/MeshData.h"
#include "Datas/PixelData.h"
#include "Resources/ResourceManager.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "Resources/Texture.h"
#include "Scene/Components/CRenderable.h"
#include "Core/Gfx/GfxResources.h"
#include "TextureImporter.h"
#include "filesystem/path.h"
#define CGLTF_IMPLEMENTATION
#include <cgltf.h>
#include <algorithm>
EFFECTS_NAMESPACE_BEGIN
// cgltf helper funcs
int getCNodeInxFromCData(const cgltf_node* node, const cgltf_data* data);
bool findAttributesType(cgltf_primitive* primitive, cgltf_attribute_type type);
bool findAttributesType(cgltf_primitive* primitive, cgltf_attribute** inAtt, cgltf_attribute_type type);
glm::mat4 getLocalMatrix(cgltf_node* node);
glm::mat4 getWorldMatrix(cgltf_node* node);
uint32_t getGLFilter(uint32_t filter);
uint32_t getGLWrap(uint32_t warp);
GltfAlphaMode getAlphaMode(cgltf_alpha_mode alphaMode);

GltfImporter::GltfImporter()
{
}

GltfImporter::~GltfImporter()
{
}

bool GltfImporter::load(const std::string& filePath)
{
    cgltf_options options = {static_cast<cgltf_file_type>(0)};
    cgltf_data* data = NULL;
    if (cgltf_parse_file(&options, filePath.c_str(), &data) != cgltf_result_success) {
        cgltf_free(data);
        return false;
    }

    if (cgltf_load_buffers(&options, data, filePath.c_str()) != cgltf_result_success) {
        cgltf_free(data);
        return false;
    }

    if (cgltf_validate(data) != cgltf_result_success) {
        cgltf_free(data);
        return false;
    }

    // load images
    std::map<cgltf_image*, std::shared_ptr<Texture>> imageHelper;
    for (int i = 0; i < data->images_count; ++i) {
        cgltf_image* CImage = &data->images[i];
        std::string imageURI = filePath + "/../" + CImage->uri;
        filesystem::path imagePath(imageURI);
        int width, height, channels;
        unsigned char* pixels = loadImage(imageURI.c_str(), width, height, channels);
        PixelDataDesc pixelDataDesc;
        pixelDataDesc.width = width;
        pixelDataDesc.height = height;
        pixelDataDesc.depth = 1;
        pixelDataDesc.array = 1;
        if (channels == 4) {
            pixelDataDesc.format = GL_RGBA8;
        }
        else if (channels == 3) {
            pixelDataDesc.format = GL_RGB8;
        }
        else {
            LOGE("error image format\n");
        }
        PixelData* pixelData = new PixelData(pixelDataDesc);
        pixelData->setData(0, pixels, width * height * channels * sizeof(unsigned char));
        std::shared_ptr<Texture> etTex = Texture::create(imagePath.filename(), pixelData);
        etTex->prepareGfxData();
        SAFE_DELETE_ARRAY(pixels);
        imageHelper[CImage] = etTex;
    }

//    // load sampler
//    std::map<cgltf_sampler*, GfxSampler*> samplerHelper;
//    for (int i = 0; i < data->samplers_count; ++i)
//    {
//        cgltf_sampler* CSampler = &data->samplers[i];
//        GfxSamplerDesc samplerDesc;
//        samplerDesc.magFilter = getGLFilter(CSampler->mag_filter);
//        samplerDesc.minFilter = getGLFilter(CSampler->min_filter);
//        samplerDesc.wrapS = getGLWrap(CSampler->wrap_s);
//        samplerDesc.wrapT = getGLWrap(CSampler->wrap_t);
//        GfxSampler* sampler = createGfxSampler(samplerDesc);
//
//        scene->samples.push_back(sampler);
//        samplerHelper[CSampler] = sampler;
//    }

    // load material
    std::map<cgltf_material*, std::shared_ptr<Material>> materialHelper;
    for (int i = 0; i < data->materials_count; ++i) {
        cgltf_material* CMaterial = &data->materials[i];

        std::shared_ptr<Material> etMaterial = Material::create(std::string(CMaterial->name) + "_Mat");
        etMaterial->setEffectType(EffectType::DEFAULT);
        etMaterial->getTextureParams()["BaseColor"] = nullptr;
        if(CMaterial->pbr_metallic_roughness.base_color_texture.texture) {
            cgltf_image* CImage = CMaterial->pbr_metallic_roughness.base_color_texture.texture->image;
            cgltf_sampler* CSampler = CMaterial->pbr_metallic_roughness.base_color_texture.texture->sampler;
            etMaterial->getTextureParams()["BaseColor"] = imageHelper[CImage];
        }

        etMaterial->getTextureParams()["Normal"] = nullptr;
        if(CMaterial->normal_texture.texture) {
            cgltf_image* CImage = CMaterial->normal_texture.texture->image;
            cgltf_sampler* CSampler = CMaterial->normal_texture.texture->sampler;
            etMaterial->getTextureParams()["Normal"] = imageHelper[CImage];
        }

        etMaterial->getTextureParams()["MetallicRoughness"] = nullptr;
        if(CMaterial->pbr_metallic_roughness.metallic_roughness_texture.texture) {
            cgltf_image* CImage = CMaterial->pbr_metallic_roughness.metallic_roughness_texture.texture->image;
            cgltf_sampler* CSampler = CMaterial->pbr_metallic_roughness.metallic_roughness_texture.texture->sampler;
            etMaterial->getTextureParams()["MetallicRoughness"] = imageHelper[CImage];
        }

        MaterialCombo alphaCombo;
        alphaCombo.fields.push_back("Opaque");
        alphaCombo.fields.push_back("Mask");
        alphaCombo.fields.push_back("Blend");
        if (CMaterial->alpha_mode == cgltf_alpha_mode_opaque) {
            alphaCombo.idx = 0;
        } else if(CMaterial->alpha_mode == cgltf_alpha_mode_mask) {
            alphaCombo.idx = 1;
        } else {
            alphaCombo.idx = 2;
        }
        etMaterial->getComboParams()["alphaMode"] = alphaCombo;

        etMaterial->getBoolParams()["DoubleSided"] = CMaterial->double_sided;

        etMaterial->getFloat4Params()["BaseColor"] = glm::make_vec4(CMaterial->pbr_metallic_roughness.base_color_factor);

        etMaterial->getFloatParams()["Metallic"] = CMaterial->pbr_metallic_roughness.metallic_factor;

        etMaterial->getFloatParams()["Roughness"] = CMaterial->pbr_metallic_roughness.roughness_factor;

        materialHelper[CMaterial] = etMaterial;
    }

    std::map<cgltf_node*, SceneNode*> nodeHelper;
    for (size_t i = 0; i < data->nodes_count; ++i)
    {
        cgltf_node* cNode = &data->nodes[i];
        SceneNode* obj = SceneNode::create(cNode->name);

        glm::vec3 translation = glm::vec3(0.0f);
        if (cNode->has_translation)
        {
            translation.x = cNode->translation[0];
            translation.y = cNode->translation[1];
            translation.z = cNode->translation[2];
        }

        glm::quat rotation = glm::quat(1, 0, 0, 0);
        if (cNode->has_rotation)
        {
            rotation.x = cNode->rotation[0];
            rotation.y = cNode->rotation[1];
            rotation.z = cNode->rotation[2];
            rotation.w = cNode->rotation[3];
        }

        glm::vec3 scale = glm::vec3(1.0f);
        if (cNode->has_scale)
        {
            scale.x = cNode->scale[0];
            scale.y = cNode->scale[1];
            scale.z = cNode->scale[2];
        }

        if (cNode->has_matrix)
        {
            glm::mat4 mat = glm::make_mat4(cNode->matrix);
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(mat, scale, rotation, translation, skew, perspective);
        }
        obj->setPosition(translation);
        obj->setEuler(glm::eulerAngles(rotation));
        obj->setScale(scale);
        nodeHelper[cNode] = obj;
    }

    for (size_t i = 0; i < data->nodes_count; ++i) {
        cgltf_node* cNode = &data->nodes[i];
        if(cNode->parent != nullptr) {
            SceneNode* currentNode = nodeHelper[cNode];
            SceneNode* parentNode = nodeHelper[cNode->parent];
            currentNode->setParent(parentNode);
        }
    }

    for (size_t i = 0; i < data->nodes_count; ++i) {
        cgltf_node* cNode = &data->nodes[i];
        cgltf_mesh* cMesh = cNode->mesh;
        cgltf_material* cMaterial = nullptr;
        SceneNode* node = nodeHelper[cNode];

        if(!cMesh) {
            continue;
        }

        uint32_t meshAttLayout = (uint32_t)SEMANTIC_POSITION;
        meshAttLayout |= (uint32_t)SEMANTIC_TEXCOORD0;
        meshAttLayout |= (uint32_t)SEMANTIC_NORMAL;
        if(findAttributesType(&cMesh->primitives[0], cgltf_attribute_type_tangent))
        {
            meshAttLayout |= (uint32_t)SEMANTIC_TANGENT;
        }

        if(findAttributesType(&cMesh->primitives[0], cgltf_attribute_type_joints))
        {
            meshAttLayout |= (uint32_t)SEMANTIC_JOINTS;
            meshAttLayout |= (uint32_t)SEMANTIC_WEIGHTS;
        }

        std::vector<float> positionBuffer;
        std::vector<float> texcoordBuffer;
        std::vector<float> normalBuffer;
        std::vector<float> tangentBuffer;
        std::vector<int> jointsBuffer;
        std::vector<float> weightsBuffer;
        std::vector<uint32_t> indexBuffer;
        uint32_t indexStart = 0;
        uint32_t indexCount = 0;

        for (int i = 0; i < cMesh->primitives_count; i++)
        {
            cgltf_primitive *cPrimitive = &cMesh->primitives[i];
            cMaterial = cPrimitive->material;
            indexStart += indexCount;
            indexCount = 0;

            // Vertices
            float *localPositionBuffer = nullptr;
            float *localTexcoordBuffer = nullptr;
            float *localNormalBuffer = nullptr;
            float *localTangentBuffer = nullptr;
            int *localJointsBuffer = nullptr;
            float *localWeightsBuffer = nullptr;

            cgltf_attribute *positionAttributes = nullptr;
            assert(findAttributesType(cPrimitive, &positionAttributes, cgltf_attribute_type_position));
            cgltf_accessor *posAccessor = positionAttributes->data;
            cgltf_buffer_view *posView = posAccessor->buffer_view;
            uint8_t *posDatas = (uint8_t *) (posView->buffer->data) + posAccessor->offset + posView->offset;
            localPositionBuffer = (float *) posDatas;

            cgltf_attribute *normalAttributes = nullptr;
            if (findAttributesType(cPrimitive, &normalAttributes, cgltf_attribute_type_normal)) {
                cgltf_accessor *normalAccessor = normalAttributes->data;
                cgltf_buffer_view *normalView = normalAccessor->buffer_view;
                uint8_t *normalDatas =
                        (uint8_t *) (normalView->buffer->data) + normalAccessor->offset + normalView->offset;
                localNormalBuffer = (float *) normalDatas;
            }


            cgltf_attribute *texcoordAttributes = nullptr;
            if (findAttributesType(cPrimitive, &texcoordAttributes, cgltf_attribute_type_texcoord)) {
                cgltf_accessor *texcoordAccessor = texcoordAttributes->data;
                cgltf_buffer_view *texcoordView = texcoordAccessor->buffer_view;
                uint8_t *texcoordDatas =
                        (uint8_t *) (texcoordView->buffer->data) + texcoordAccessor->offset + texcoordView->offset;
                localTexcoordBuffer = (float *) texcoordDatas;
            }

            cgltf_attribute *tangentAttributes = nullptr;
            if (findAttributesType(cPrimitive, &tangentAttributes, cgltf_attribute_type_tangent)) {
                cgltf_accessor *tangentAccessor = tangentAttributes->data;
                cgltf_buffer_view *tangentView = tangentAccessor->buffer_view;
                uint8_t *tangentDatas =
                        (uint8_t *) (tangentView->buffer->data) + tangentAccessor->offset + tangentView->offset;
                localTangentBuffer = (float *) tangentDatas;
            }

            cgltf_attribute *jointsAttributes = nullptr;
            if (findAttributesType(cPrimitive, &jointsAttributes, cgltf_attribute_type_joints)) {
                cgltf_accessor *jointsAccessor = jointsAttributes->data;
                cgltf_buffer_view *jointsView = jointsAccessor->buffer_view;
                uint8_t *jointsDatas =
                        (uint8_t *) (jointsView->buffer->data) + jointsAccessor->offset + jointsView->offset;
                localJointsBuffer = (int *) jointsDatas;
            }

            cgltf_attribute *weightsAttributes = nullptr;
            if (findAttributesType(cPrimitive, &weightsAttributes, cgltf_attribute_type_weights)) {
                cgltf_accessor *weightsAccessor = weightsAttributes->data;
                cgltf_buffer_view *weightsView = weightsAccessor->buffer_view;
                uint8_t *weightsDatas =
                        (uint8_t *) (weightsView->buffer->data) + weightsAccessor->offset + weightsView->offset;
                localWeightsBuffer = (float *) weightsDatas;
            }

            for (size_t v = 0; v < posAccessor->count; v++)
            {
                positionBuffer.push_back(localPositionBuffer[v * 3]);
                positionBuffer.push_back(localPositionBuffer[v * 3 + 1]);
                positionBuffer.push_back(localPositionBuffer[v * 3 + 2]);

                if(localTexcoordBuffer)
                {
                    texcoordBuffer.push_back(localTexcoordBuffer[v * 2]);
                    texcoordBuffer.push_back(localTexcoordBuffer[v * 2 + 1]);
                } else
                {
                    texcoordBuffer.push_back(0.0f);
                    texcoordBuffer.push_back(0.0f);
                }

                if(localNormalBuffer)
                {
                    normalBuffer.push_back(localNormalBuffer[v * 3]);
                    normalBuffer.push_back(localNormalBuffer[v * 3 + 1]);
                    normalBuffer.push_back(localNormalBuffer[v * 3 + 2]);
                } else
                {
                    normalBuffer.push_back(0.0f);
                    normalBuffer.push_back(0.0f);
                    normalBuffer.push_back(0.0f);
                }

                if ((meshAttLayout & (uint32_t)SEMANTIC_TANGENT) != 0)
                {
                    tangentBuffer.push_back(localTangentBuffer[v * 3]);
                    tangentBuffer.push_back(localTangentBuffer[v * 3 + 1]);
                    tangentBuffer.push_back(localTangentBuffer[v * 3 + 2]);
                }

                if((meshAttLayout & (uint32_t)SEMANTIC_JOINTS) != 0)
                {
                    jointsBuffer.push_back(localJointsBuffer[v * 4]);
                    jointsBuffer.push_back(localJointsBuffer[v * 4 + 1]);
                    jointsBuffer.push_back(localJointsBuffer[v * 4 + 2]);
                    jointsBuffer.push_back(localJointsBuffer[v * 4 + 3]);

                    weightsBuffer.push_back(localWeightsBuffer[v * 4]);
                    weightsBuffer.push_back(localWeightsBuffer[v * 4 + 1]);
                    weightsBuffer.push_back(localWeightsBuffer[v * 4 + 2]);
                    weightsBuffer.push_back(localWeightsBuffer[v * 4 + 3]);
                }
            }

            // Indices
            cgltf_accessor* cIndexAccessor = cPrimitive->indices;
            cgltf_buffer_view* cIndexBufferView = cIndexAccessor->buffer_view;
            cgltf_buffer * cIndexBuffer = cIndexBufferView->buffer;

            indexCount = static_cast<uint32_t>(cIndexAccessor->count);

            switch (cIndexAccessor->component_type)
            {
                case cgltf_component_type_r_32u:
                {
                    uint32_t *buf = new uint32_t[indexCount];
                    uint8_t *src = (uint8_t *)cIndexBuffer->data + cIndexAccessor->offset + cIndexBufferView->offset;
                    memcpy(buf, src, indexCount * sizeof(uint32_t));
                    for (size_t index = 0; index < indexCount; index++)
                    {
                        indexBuffer.push_back(buf[index]);
                    }
                    break;
                }

                case cgltf_component_type_r_16u:
                {
                    uint16_t *buf = new uint16_t[indexCount];
                    uint8_t *src = (uint8_t *) cIndexBuffer->data + cIndexAccessor->offset + cIndexBufferView->offset;
                    memcpy(buf, src, indexCount * sizeof(uint16_t));
                    for (size_t index = 0; index < indexCount; index++)
                    {
                        indexBuffer.push_back(buf[index]);
                    }
                    break;
                }
                case cgltf_component_type_r_8u:
                {
                    uint8_t *buf = new uint8_t[indexCount];
                    uint8_t *src = (uint8_t *) cIndexBuffer->data + cIndexAccessor->offset + cIndexBufferView->offset;
                    memcpy(buf, src, indexCount * sizeof(uint8_t));
                    for (size_t index = 0; index < indexCount; index++)
                    {
                        indexBuffer.push_back(buf[index]);
                    }
                    break;
                }
                default:
                    printf("Index component type not supported! \n");
            }
        }

        VertexLayout* vertexLayout = new VertexLayout(meshAttLayout);
        MeshData* meshData = new MeshData(positionBuffer.size() / 3, indexBuffer.size(), vertexLayout);
        meshData->setIndexes(indexBuffer.data(), indexBuffer.size() * sizeof(uint32_t));

        meshData->setAttribute(SEMANTIC_POSITION, positionBuffer.data(), positionBuffer.size() * sizeof(float));
        meshData->setAttribute(SEMANTIC_TEXCOORD0, texcoordBuffer.data(), texcoordBuffer.size() * sizeof(float));
        meshData->setAttribute(SEMANTIC_NORMAL, normalBuffer.data(), normalBuffer.size() * sizeof(float));
        if ((meshAttLayout & (uint32_t)SEMANTIC_TANGENT) != 0) {
            meshData->setAttribute(SEMANTIC_TANGENT, tangentBuffer.data(), tangentBuffer.size() * sizeof(float));
        }

        if((meshAttLayout & (uint32_t)SEMANTIC_JOINTS) != 0) {
            meshData->setAttribute(SEMANTIC_JOINTS, jointsBuffer.data(), jointsBuffer.size() * sizeof(int));
            meshData->setAttribute(SEMANTIC_WEIGHTS, weightsBuffer.data(), weightsBuffer.size() * sizeof(float));
        }

        std::shared_ptr<Mesh> etMesh = Mesh::create(std::string(cMesh->name) + "_Mesh", meshData);
        etMesh->prepareGfxData();
        CRenderable* crenderable = node->addComponent<CRenderable>();
        crenderable->setMesh(etMesh);
        crenderable->setMaterial(materialHelper[cMaterial]);
    }

    cgltf_free(data);
}

int getCNodeInxFromCData(const cgltf_node* node, const cgltf_data* data)
{
    for (size_t i = 0; i < data->nodes_count; ++i)
    {
        if(&data->nodes[i] == node)
        {
            return i;
        }
    }
    return -1;
}

bool findAttributesType(cgltf_primitive* primitive, cgltf_attribute_type type)
{
    for (int i = 0; i < primitive->attributes_count; i++)
    {
        cgltf_attribute* att = &primitive->attributes[i];
        if(att->type == type)
        {
            return true;
        }
    }
    return false;
}

bool findAttributesType(cgltf_primitive* primitive, cgltf_attribute** inAtt, cgltf_attribute_type type)
{
    for (int i = 0; i < primitive->attributes_count; i++)
    {
        cgltf_attribute* att = &primitive->attributes[i];
        if(att->type == type)
        {
            *inAtt = att;
            return true;
        }
    }
    return false;
}

glm::mat4 getLocalMatrix(cgltf_node* node)
{
    glm::vec3 translation = glm::vec3(0.0f);
    if (node->has_translation)
    {
        translation.x = node->translation[0];
        translation.y = node->translation[1];
        translation.z = node->translation[2];
    }

    glm::quat rotation = glm::quat(1, 0, 0, 0);
    if (node->has_rotation)
    {
        rotation.x = node->rotation[0];
        rotation.y = node->rotation[1];
        rotation.z = node->rotation[2];
        rotation.w = node->rotation[3];
    }

    glm::vec3 scale = glm::vec3(1.0f);
    if (node->has_scale)
    {
        scale.x = node->scale[0];
        scale.y = node->scale[1];
        scale.z = node->scale[2];
    }

    glm::mat4 r, t, s;
    r = glm::toMat4(rotation);
    t = glm::translate(glm::mat4(1.0), translation);
    s = glm::scale(glm::mat4(1.0), scale);
    return t * r * s;
}

glm::mat4 getWorldMatrix(cgltf_node* node)
{
    cgltf_node* curNode = node;
    glm::mat4 out = getLocalMatrix(curNode);

    while (curNode->parent != nullptr)
    {
        curNode = node->parent;
        out = getLocalMatrix(curNode) * out;
    }
    return out;
}

void destroyGltfScene(GltfScene* scene)
{
    for (int i = 0; i < scene->meshs.size(); ++i)
    {
        delete scene->meshs[i];
    }
    scene->meshs.clear();
    for (int i = 0; i < scene->materials.size(); ++i)
    {
        delete scene->materials[i];
    }
    scene->materials.clear();

    for (int i = 0; i < scene->textures.size(); ++i)
    {
        destroyGfxTexture(scene->textures[i]);
    }
    scene->textures.clear();

    for (int i = 0; i < scene->samples.size(); ++i)
    {
        destroyGfxSampler(scene->samples[i]);
    }
    scene->samples.clear();
}

    uint32_t getGLFilter(uint32_t filter)
    {
        switch (filter)
        {
            case 9728:
                return GL_NEAREST;
            case 9729:
                return GL_LINEAR;
//            case 9984:
//                return GL_NEAREST_MIPMAP_NEAREST;
//            case 9985:
//                return GL_LINEAR_MIPMAP_NEAREST;
//            case 9986:
//                return GL_NEAREST_MIPMAP_LINEAR;
//            case 9987:
//                return GL_LINEAR_MIPMAP_LINEAR;
            default:
                return GL_LINEAR;
        }
    }

    uint32_t getGLWrap(uint32_t warp)
    {
        switch (warp)
        {
            case 10497:
                return GL_REPEAT;
            case 33071:
                return GL_CLAMP_TO_EDGE;
            case 33648 :
                return GL_MIRRORED_REPEAT;
            default:
                return GL_REPEAT;
        }
    }

    GltfAlphaMode getAlphaMode(cgltf_alpha_mode alphaMode)
    {
        switch (alphaMode)
        {
            case cgltf_alpha_mode_opaque:
                return ALPHA_MODE_OPAQUE;
            case cgltf_alpha_mode_mask:
                return ALPHA_MODE_MASK;
            case cgltf_alpha_mode_blend :
                return ALPHA_MODE_BLEND;
            default:
                return ALPHA_MODE_OPAQUE;
        }
    }

EFFECTS_NAMESPACE_END