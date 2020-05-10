#pragma once
#include "../Base.h"
#include "../Gfx/Gfx.h"
#include "../../Math/Math.h"
#include "../../Math/Geometry.h"
#include <vector>
#include <map>
#include <string>
EFFECTS_NAMESPACE_BEGIN

namespace temp {
    struct Node {
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

    struct Material {
        glm::vec4 base_color;
        float metallic;
        float roughness;
    };

    struct Mesh {
        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 uv;
        };
        std::vector<Vertex> vertices;
        std::vector<int> indices;
        std::vector<Triangle> tris;
        int material_id;
        GLuint vao;
        GLuint vbo;
        GLuint ibo;
    };

    void initMesh(Mesh *mesh);

    void drawMesh(Mesh *mesh);

    void genMeshTriangles(Mesh *mesh, const glm::mat4& world);

    void releaseMesh(Mesh *mesh);

    class Scene {
    public:
        Scene();

        ~Scene();

        void load(std::string file_path);

        glm::mat4 getLocalMatrix(int id);

        glm::mat4 getWorldMatrix(int id);

        Node getNode(int id);

        void printNodeInfo(int id);

    public:
        std::vector<Mesh *> m_meshs;
        std::vector<Material> m_materials;
        std::vector<Node> m_nodes;
    };
};
EFFECTS_NAMESPACE_END