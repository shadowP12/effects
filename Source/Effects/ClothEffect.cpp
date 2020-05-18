#include "ClothEffect.h"
#include "../Core/Utility/FileUtility.h"
#include "../Core/Gfx/GfxResources.h"
#include "../Core/Gfx/GfxRender.h"
#include "../Core/Renderer/RenderHelper.h"
#include "../Core/Datas/MeshData.h"
#include "../Core/RenderResources/Mesh.h"

EFFECTS_NAMESPACE_BEGIN
    struct Mass
    {
        Mass(const glm::vec3& inPosition, const float& inMass, const bool& inPinned)
        {
            startPosition = inPosition;
            position = inPosition;
            lastPosition = inPosition;
            mass = inMass;
            pinned = inPinned;
        }
        float mass;
        bool pinned;
        glm::vec3 startPosition;
        glm::vec3 position;
        glm::vec3 lastPosition;
        glm::vec3 velocity;
        glm::vec3 forces;
    };

    struct Spring
    {
        Spring(Mass* inM1, Mass* inM2, float inK)
        {
            m1 = inM1;
            m2 = inM2;
            k = inK;
            restLength = glm::length(m1->position - m2->position);
        }

        float k;
        double restLength;
        Mass* m1;
        Mass* m2;
    };

    class Rope
    {
    public:
        Rope(const glm::vec3& start, const glm::vec3& end, int numNodes, float nodeMass, float k,
             const std::vector<bool>& pinnedNodes)
        {
            // create masses
            glm::vec3 dir = glm::normalize(end - start);
            float len = glm::length(end - start);
            for (int i = 0; i < numNodes; ++i)
            {
                glm::vec3 point = dir * (len*i/numNodes);
                Mass* mass = new Mass(point, nodeMass, pinnedNodes[i]);
                masses.push_back(mass);
            }

            // create springs
            for (int i = 0; i < masses.size(); ++i)
            {
                if(i == 0 || i == (masses.size()-1))
                {
                    continue;
                }
                Spring* spring = new Spring(masses[i-1], masses[i+1], k);
                springs.push_back(spring);
            }
        }
        ~Rope()
        {
            for (int i = 0; i < masses.size(); ++i)
            {
                delete masses[i];
            }

            for (int i = 0; i < springs.size(); ++i)
            {
                delete springs[i];
            }
        }

        void simulateVerlet(float delta, glm::vec3 gravity)
        {

        }
        void simulateEuler(float delta, glm::vec3 gravity)
        {

        }

        std::vector<Mass*> masses;
        std::vector<Spring*> springs;
    };

    static GfxProgram* gRopeProgram = nullptr;
    static GfxBuffer* gRopeVertexBuffer = nullptr;
    static MeshDataDescription* gRopeVertexLayout = nullptr;
    static float* gRopeVertices = nullptr;
    static GfxRenderObj gfxRenderObj;
    static Rope* gRope = nullptr;
    static int gRopeNodeCount = 6;

    ClothEffect::ClothEffect(int width, int height)
            :BaseEffect(width, height)
    {
    }

    ClothEffect::~ClothEffect()
    {
        if(gRopeVertices)
            delete[] gRopeVertices;
        if(gRopeProgram)
            delete gRopeProgram;
        if(gRopeVertexBuffer)
            delete gRopeVertexBuffer;
        if(gRopeVertexLayout)
            delete gRopeVertexLayout;
        if(gRope)
            delete gRope;
    }

    void ClothEffect::prepare()
    {
        uint32_t meshAttLayout = (uint32_t)MAL_POSITION;
        gRopeVertexLayout = new MeshDataDescription(meshAttLayout);

        GfxBufferDesc vertexBufferDesc;
        vertexBufferDesc.size = 0;
        vertexBufferDesc.memUsage = GfxMemoryUsageBit::HOST;
        vertexBufferDesc.bufferUsage = GfxBufferUsageBit::VERTEX;
        vertexBufferDesc.bufferAccess = GfxBufferAccessBit::WRITE;
        gRopeVertexBuffer = new GfxBuffer(vertexBufferDesc);

        gRopeProgram = loadProgram(getCurrentPath() + R"(\BuiltinResources\Shaders\cloth\rope.vs)",
                                  getCurrentPath() + R"(\BuiltinResources\Shaders\cloth\rope.fs)");

        gRopeVertices = new float[gRopeNodeCount*3];

        std::vector<bool> pinnedNodes;
        for (int i = 0; i < gRopeNodeCount; ++i)
        {
            pinnedNodes.push_back(true);
        }
        gRope = new Rope(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                    gRopeNodeCount, 5.0f, 0.2f, pinnedNodes);
    }

    void ClothEffect::update(float t)
    {
        BaseEffect::update(t);
        for (int i = 0; i < gRopeNodeCount; ++i)
        {
            gRopeVertices[i*3] = gRope->masses[i]->position.x;
            gRopeVertices[i*3+1] = gRope->masses[i]->position.y;
            gRopeVertices[i*3+2] = gRope->masses[i]->position.z;
        }
    }

    void ClothEffect::render()
    {
        glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gRopeVertexBuffer->writeData(gRopeVertices, gRopeNodeCount * 3 * sizeof(float));
        glm::vec3 ropeColor1 = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 ropeColor2 = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 identityMat = glm::mat4(1.0f);
        gRopeProgram->setFloat3("u_color", &ropeColor1[0]);
        gRopeProgram->setMat4("u_model", &identityMat[0][0]);
        gRopeProgram->setMat4("u_view", &identityMat[0][0]);
        gRopeProgram->setMat4("u_proj", &identityMat[0][0]);
        gfxRenderObj.setVertexBuffer(gRopeVertexBuffer);
        gfxRenderObj.setProgram(gRopeProgram);
        gfxRenderObj.setVertexLayout(gRopeVertexLayout);
        gfxRenderObj.draw(GfxPrimitiveMode::LINE_STRIP, 0, gRopeNodeCount);
        gRopeProgram->setFloat3("u_color", &ropeColor2[0]);
        glPointSize(10.0f);
        gfxRenderObj.draw(GfxPrimitiveMode::POINT_LIST, 0, gRopeNodeCount);
        glPointSize(1.0f);
    }

EFFECTS_NAMESPACE_END
