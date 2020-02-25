#include "GltfImporter.h"
#include "../Core/Scene/Scene.h"
#include "../Core/Datas/MeshData.h"
#include "../Core/RenderResources/Mesh.h"
#define CGLTF_IMPLEMENTATION
#include <cgltf.h>
EFFECTS_NAMESPACE_BEGIN
// cgltf helper funcs
int getCNodeInxFromCScene(const cgltf_node* node, const cgltf_scene* scene);
bool findAttributesType(cgltf_primitive* primitive, cgltf_attribute_type type);
bool findAttributesType(cgltf_primitive* primitive, cgltf_attribute** inAtt, cgltf_attribute_type type);

GltfImporter::GltfImporter()
{
}

GltfImporter::~GltfImporter()
{
}

void GltfImporter::load(std::string filePath, Scene* scene)
{
    cgltf_options options = {static_cast<cgltf_file_type>(0)};
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, filePath.c_str(), &data);
    if (result == cgltf_result_success)
        result = cgltf_load_buffers(&options, data, filePath.c_str());

    if (result == cgltf_result_success)
        result = cgltf_validate(data);

    std::vector<std::shared_ptr<SceneObject>> objs;
    cgltf_scene* cScene = data->scene;
    for (size_t i = 0; i < cScene->nodes_count; ++i)
    {
        cgltf_node* cNode = cScene->nodes[i];
        std::shared_ptr<SceneObject> obj = std::make_shared<SceneObject>();

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
        obj->setTransform(translation, scale, rotation);
        objs.push_back(obj);
    }

    for (size_t i = 0; i < cScene->nodes_count; ++i)
    {
        cgltf_node* cNode = cScene->nodes[i];
        std::shared_ptr<SceneObject> obj = objs[i];
        if(cNode->parent != nullptr)
        {
            int parentInx = getCNodeInxFromCScene(cNode, cScene);
            if(parentInx != -1)
            {
                std::shared_ptr<SceneObject> parentObj = objs[parentInx];
                obj->setParent(parentObj);
            }
        }
    }

    for (size_t i = 0; i < cScene->nodes_count; ++i)
    {
        cgltf_node* cNode = cScene->nodes[i];
        cgltf_mesh* cMesh = cNode->mesh;

        if(cMesh->primitives_count <= 0)
        {
            continue;
        }
        uint32_t meshAttLayout = (uint32_t)MAL_POSITION;
        meshAttLayout |= (uint32_t)MAL_TEXCOORD0;
        meshAttLayout |= (uint32_t)MAL_NORMAL;
        if(findAttributesType(&cMesh->primitives[0], cgltf_attribute_type_tangent))
        {
            meshAttLayout |= (uint32_t)MAL_TANGENT;
        }

        if(findAttributesType(&cMesh->primitives[0], cgltf_attribute_type_joints))
        {
            meshAttLayout |= (uint32_t)MAL_BLEND_WEIGHTS;
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

                if ((meshAttLayout & (uint32_t)MAL_TANGENT) != 0)
                {
                    tangentBuffer.push_back(localTangentBuffer[v * 3]);
                    tangentBuffer.push_back(localTangentBuffer[v * 3 + 1]);
                    tangentBuffer.push_back(localTangentBuffer[v * 3 + 2]);
                }

                if((meshAttLayout & (uint32_t)MAL_BLEND_WEIGHTS) != 0)
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

        positionBuffer;
        MeshDataDescription* meshDataDesc = new MeshDataDescription(meshAttLayout);
        MeshData* meshData = new MeshData(positionBuffer.size() / 3, indexBuffer.size(), meshDataDesc);
        meshData->setIndexes(indexBuffer.data(), indexBuffer.size() * sizeof(uint32_t));
        meshData->setAttribute(MAS_POSITION, positionBuffer.data(), positionBuffer.size() * sizeof(float));
        meshData->setAttribute(MAS_TEXCOORD, texcoordBuffer.data(), texcoordBuffer.size() * sizeof(float));
        meshData->setAttribute(MAS_NORMAL, normalBuffer.data(), normalBuffer.size() * sizeof(float));
        if ((meshAttLayout & (uint32_t)MAL_TANGENT) != 0)
        {
            meshData->setAttribute(MAS_TANGENT, tangentBuffer.data(), tangentBuffer.size() * sizeof(float));
        }

        if((meshAttLayout & (uint32_t)MAL_BLEND_WEIGHTS) != 0)
        {
            meshData->setAttribute(MAS_BLEND_INDICES, jointsBuffer.data(), jointsBuffer.size() * sizeof(int));
            meshData->setAttribute(MAS_BLEND_WEIGHTS, weightsBuffer.data(), weightsBuffer.size() * sizeof(float));
        }

        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(meshData);
        mesh->initialize();


    }

    cgltf_free(data);
}

int getCNodeInxFromCScene(const cgltf_node* node, const cgltf_scene* scene)
{
    for (size_t i = 0; i < scene->nodes_count; ++i)
    {
        if(scene->nodes[i] == node)
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

EFFECTS_NAMESPACE_END