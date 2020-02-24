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
        MeshDataDescription* meshDataDesc = new MeshDataDescription();
        if(cMesh->primitives_count <= 0)
        {
            delete meshDataDesc;
            continue;
        }

        meshDataDesc->addMeshAttribute(MAT_FLOAT3, MAS_POSITION);
        meshDataDesc->addMeshAttribute(MAT_FLOAT2, MAS_TEXCOORD);
        meshDataDesc->addMeshAttribute(MAT_FLOAT3, MAS_NORMAL);
        if(findAttributesType(&cMesh->primitives[0], cgltf_attribute_type_tangent))
        {
            meshDataDesc->addMeshAttribute(MAT_FLOAT3, MAS_TANGENT);
        }

        if(findAttributesType(&cMesh->primitives[0], cgltf_attribute_type_joints))
        {
            meshDataDesc->addMeshAttribute(MAT_FLOAT3, MAS_BLEND_INDICES);
            meshDataDesc->addMeshAttribute(MAT_FLOAT3, MAS_BLEND_WEIGHTS);
        }



        delete meshDataDesc;
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

EFFECTS_NAMESPACE_END