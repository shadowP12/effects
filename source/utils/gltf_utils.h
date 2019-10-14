#pragma once
#include "base.h"
#include "gl_utils.h"
#include "utils.h"
struct GltfNode
{
	std::string node_name;
	int node_id;
	int parent;
	std::vector<int> childrens;
	int mesh;
	int light;
	glm::vec3 translation;
	glm::vec3 scale;
	glm::quat rotation;
};

struct GltfMaterial
{
	glm::vec4 base_color;
	float metallic;
	float roughness;
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

//1:GL_TRIANGLES
//2:GL_TRIANGLE_STRIP
//3:GL_LINE
void draw_gltf_mesh(GltfMesh* mesh, int draw_type);

struct GltfScene
{
	std::vector<GltfMesh*> meshs;
	std::vector<GltfMaterial> materials;
	std::map<int, GltfNode> nodes;
};

GltfScene* load_gltf_scene(std::string file);

void delete_gltf_scene(GltfScene* scene);

//获取节点的局部矩阵
glm::mat4 get_local_matrix(GltfScene* scene, int id);

//获取节点的世界矩阵
glm::mat4 get_world_matrix(GltfScene* scene, int id);

GltfNode& get_gltf_node(GltfScene* scene, int id);