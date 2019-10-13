#include "gltf_utils.h"
#define TINYGLTF_IMPLEMENTATION
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
	//加载节点数据
	for (int i = 0; i < gltf_model.nodes.size(); i++)
	{
		const tinygltf::Node& gltf_node = gltf_model.nodes[i];
		GltfNode node;
		node.node_id = i;
		node.parent = -1;
		glm::vec3 translation = glm::vec3(0.0f);
		if (gltf_node.translation.size() == 3)
		{
			translation = glm::make_vec3(gltf_node.translation.data());
		}

		glm::quat rotation = glm::quat(1, 0, 0, 0);
		if (gltf_node.rotation.size() == 4)
		{
			glm::quat rotation = glm::make_quat(gltf_node.rotation.data());
		}

		glm::vec3 scale = glm::vec3(1.0f);
		if (gltf_node.scale.size() == 3)
		{
			scale = glm::make_vec3(gltf_node.scale.data());
		}

		node.translation = translation;
		node.scale = scale;
		node.rotation = rotation;
		res->nodes[node.node_id] = node;
		node.mesh = -1;
		if (gltf_node.mesh > -1)
		{
			node.mesh = gltf_node.mesh;
		}
	}
	//加载网格数据
	for (int i = 0; i < gltf_model.meshes.size(); i++)
	{
		const tinygltf::Mesh gltf_mesh = gltf_model.meshes[i];
		for (int j = 0; j < gltf_mesh.primitives.size(); j++)
		{
			const tinygltf::Primitive &gltf_primitive = gltf_mesh.primitives[j];

			const float *position_buffer = nullptr;
			const float *normal_buffer = nullptr;
			const float *uv_buffer = nullptr;

			assert(gltf_primitive.attributes.find("POSITION") != gltf_primitive.attributes.end());

			const tinygltf::Accessor &position_accessor = gltf_model.accessors[gltf_primitive.attributes.find("POSITION")->second];
			const tinygltf::BufferView &position_buffer_view = gltf_model.bufferViews[position_accessor.bufferView];
			position_buffer = reinterpret_cast<const float *>(&(gltf_model.buffers[position_buffer_view.buffer].data[position_accessor.byteOffset + position_buffer_view.byteOffset]));

			if (gltf_primitive.attributes.find("NORMAL") != gltf_primitive.attributes.end())
			{
				const tinygltf::Accessor &normal_accessor = gltf_model.accessors[gltf_primitive.attributes.find("NORMAL")->second];
				const tinygltf::BufferView &normal_buffer_view = gltf_model.bufferViews[normal_accessor.bufferView];
				normal_buffer = reinterpret_cast<const float *>(&(gltf_model.buffers[normal_buffer_view.buffer].data[normal_accessor.byteOffset + normal_buffer_view.byteOffset]));
			}

			if (gltf_primitive.attributes.find("TEXCOORD_0") != gltf_primitive.attributes.end())
			{
				const tinygltf::Accessor &uv_accessor = gltf_model.accessors[gltf_primitive.attributes.find("TEXCOORD_0")->second];
				const tinygltf::BufferView &uv_buffer_view = gltf_model.bufferViews[uv_accessor.bufferView];
				uv_buffer = reinterpret_cast<const float *>(&(gltf_model.buffers[uv_buffer_view.buffer].data[uv_accessor.byteOffset + uv_buffer_view.byteOffset]));
			}

			GltfMesh* mesh = new GltfMesh();
			for (int v = 0; v < position_accessor.count; v++)
			{
				GltfMesh::Vertex vertex;
				vertex.position = glm::make_vec3(&position_buffer[v * 3]);
				if (normal_buffer)
				{
					vertex.normal = glm::make_vec3(&normal_buffer[v * 3]);
				}
				else
				{
					vertex.normal = glm::vec3(0.0, 0.0, 0.0);
				}

				if (uv_buffer)
				{
					vertex.uv = glm::make_vec2(&uv_buffer[v * 2]);
				}
				else
				{
					vertex.uv = glm::vec2(0.0, 0.0);
				}
				mesh->vertices.push_back(vertex);
			}

			const tinygltf::Accessor &index_accessor = gltf_model.accessors[gltf_primitive.indices];
			const tinygltf::BufferView &index_buffer_view = gltf_model.bufferViews[index_accessor.bufferView];
			const tinygltf::Buffer &index_buffer = gltf_model.buffers[index_buffer_view.buffer];

			switch (index_accessor.componentType)
			{
			case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:
			{
				uint32_t *buf = new uint32_t[index_accessor.count];
				memcpy(buf, &index_buffer.data[index_accessor.byteOffset + index_buffer_view.byteOffset], index_accessor.count * sizeof(uint32_t));
				for (size_t index = 0; index < index_accessor.count; index++)
				{
					mesh->indices.push_back(buf[index]);
				}
				break;
			}
			case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT:
			{
				uint16_t *buf = new uint16_t[index_accessor.count];
				memcpy(buf, &index_buffer.data[index_accessor.byteOffset + index_buffer_view.byteOffset], index_accessor.count * sizeof(uint16_t));
				for (size_t index = 0; index < index_accessor.count; index++)
				{
					mesh->indices.push_back(buf[index]);
				}
				break;

			}
			case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:
			{
				uint8_t *buf = new uint8_t[index_accessor.count];
				memcpy(buf, &index_buffer.data[index_accessor.byteOffset + index_buffer_view.byteOffset], index_accessor.count * sizeof(uint8_t));
				for (size_t index = 0; index < index_accessor.count; index++)
				{
					mesh->indices.push_back(buf[index]);
				}
				break;
			}
			default:
				std::cerr << "Index component type " << index_accessor.componentType << " not supported!" << std::endl;
			}
			mesh->material_id = gltf_primitive.material;
			res->meshs.push_back(mesh);
		}
	}
	//加载图片资源
	for (int i = 0; i < gltf_model.images.size(); i++)
	{
		const tinygltf::Image& gltf_image = gltf_model.images[i];

	}
	//加载材质数据

	//设置节点的数据
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
	//释放所有gltf资源
	for (int i = 0; i < scene->meshs.size(); i++)
	{
		if (scene->meshs[i])
		{
			GltfMesh* mesh = scene->meshs[i];
			glDeleteVertexArrays(1, &mesh->vao);
			glDeleteBuffers(1, &mesh->vbo);
			glDeleteBuffers(1, &mesh->ibo);
			delete mesh;
		}
	}
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

void init_gltf_mesh(GltfMesh* mesh)
{
	//初始化gltf mesh的gpu资源
	glGenVertexArrays(1, &mesh->vao);
	glGenBuffers(1, &mesh->vbo);
	glGenBuffers(1, &mesh->ibo);

	glBindVertexArray(mesh->vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(GltfMesh::Vertex), &mesh->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(int), &mesh->indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GltfMesh::Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GltfMesh::Vertex), (void*)offsetof(GltfMesh::Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GltfMesh::Vertex), (void*)offsetof(GltfMesh::Vertex, uv));
	glBindVertexArray(0);
}

void draw_gltf_mesh(GltfMesh* mesh, int draw_type)
{
	glBindVertexArray(mesh->vao);
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
