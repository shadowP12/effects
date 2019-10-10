#include "gltf_utils.h"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "tiny_gltf.h"


GltfScene* load_gltf_scene(std::string file)
{
	std::string error;
	tinygltf::Model gltf_model;
	tinygltf::TinyGLTF gltf_context;
	bool loaded = gltf_context.LoadASCIIFromFile(&gltf_model, &error, file.c_str());
	if (!loaded)
	{
		LOGE("%s\n",error.c_str());
		return nullptr;
	}
	
	GltfScene* res = new GltfScene();
	for (int i = 0; i < gltf_model.nodes.size(); i++)
	{
		const tinygltf::Node& gltf_node = gltf_model.nodes[i];
		GltfNode node;
		node.node_id = i;
		node.parent = -1;
		node.translation = glm::make_vec3(gltf_node.translation.data());
		node.scale = glm::make_vec3(gltf_node.scale.data());
		node.rotation = glm::make_quat(gltf_node.rotation.data());
		res->nodes[node.node_id] = node;
	}
	//设置节点的层次关系
	for (int i = 0; i < gltf_model.nodes.size(); i++)
	{
		const tinygltf::Node& gltf_node = gltf_model.nodes[i];
		GltfNode& node = res->nodes[i];
		for (int j = 0; j < gltf_node.children.size(); j++)
		{
			GltfNode& children_node = res->nodes[j];
			children_node.parent = node.node_id;
			node.childrens.push_back(gltf_node.children[0]);
		}
	}
	return res;
}

void delete_gltf_scene(GltfScene* scene)
{
	if (scene)
		delete scene;
}

glm::mat4 get_local_matrix(GltfScene* scene, int id)
{
	GltfNode& node = scene->nodes[id];
	glm::mat4 r, t, s;
	r = glm::toMat4(node.rotation);
	t = glm::translate(glm::mat4(1.0), node.translation);
	s = glm::scale(glm::mat4(1.0), node.scale);
	return t*r*s;
}

glm::mat4 get_world_matrix(GltfScene* scene, int id)
{
	GltfNode& node = scene->nodes[id];
	glm::mat4 out = get_local_matrix(scene, id);

	int parent_id = node.parent;
	while (parent_id != -1)
	{
		GltfNode& cur_node = scene->nodes[parent_id];
		out = get_local_matrix(scene, parent_id) * out;
		parent_id = cur_node.parent;
	}
	return out;
}