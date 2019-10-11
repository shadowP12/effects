#pragma once
#include "base.h"
#include "gl_utils.h"
#include "utils.h"
struct GltfNode
{
	int node_id;
	int parent;
	std::vector<int> childrens;
	glm::vec3 translation;
	glm::vec3 scale;
	glm::quat rotation;
};

struct GltfMesh
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	int material_id;
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
};

void init_gltf_mesh(GltfMesh* mesh);

struct GltfScene
{
	std::vector<GltfMesh*> meshs;
	std::map<int, GltfNode> nodes;
};

GltfScene* load_gltf_scene(std::string file);

void delete_gltf_scene(GltfScene* scene);

//获取节点的局部矩阵
glm::mat4 get_local_matrix(GltfScene* scene, int id);

//获取节点的世界矩阵
glm::mat4 get_world_matrix(GltfScene* scene, int id);