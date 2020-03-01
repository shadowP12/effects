#include "DebugEffect.h"
#include "../Core/Utility/FileUtility.h"
#include "../Core/Gfx/GpuProgram.h"
#include "../Core/Gfx/GfxDebug.h"
#include "../Core/Scene/RenderScene.h"
#include "../Math/Geometry.h"
#include "../Core/Renderer/Renderer.h"
#include "../Core/Renderer/Renderable.h"
#include "../Core/RenderResources/Mesh.h"
#include "../Core/RenderResources/Material.h"

EFFECTS_NAMESPACE_BEGIN

DebugEffect::DebugEffect(int width, int height)
	:BaseEffect(width, height)
{
}
DebugEffect::~DebugEffect()
{
	delete m_debug_program;
	delete m_debug_lines;
	temp::releaseMesh(m_plane_mesh);
}

void DebugEffect::prepare()
{
	std::string vs_path = getCurrentPath() + "\\BuiltinResources\\Shaders\\debug.vs";
	std::string vs;
	readFileData(vs_path, vs);
	std::string fs_path = getCurrentPath() + "\\BuiltinResources\\Shaders\\debug.fs";
	std::string fs;
	readFileData(fs_path, fs);
	m_debug_program = new GpuProgram(vs, fs);

	m_debug_lines = new DebugLines();
	m_debug_lines->addLine(&glm::vec3(0.0, 0.0, 0.0)[0], &glm::vec3(10.0, 0.0, 0.0)[0], &glm::vec4(1.0, 0.0, 0.0, 0.0)[0]);
	m_debug_lines->addLine(&glm::vec3(0.0, 0.0, 0.0)[0], &glm::vec3(0.0, 10.0, 0.0)[0], &glm::vec4(0.0, 1.0, 0.0, 0.0)[0]);
	m_debug_lines->addLine(&glm::vec3(0.0, 0.0, 0.0)[0], &glm::vec3(0.0, 0.0, 10.0)[0], &glm::vec4(0.0, 0.0, 1.0, 0.0)[0]);

	m_plane_mesh = new temp::Mesh();
	preparePlaneMesh();
	temp::initMesh(m_plane_mesh);
}

glm::vec3 randomInUnitSphere(unsigned int* seed0, unsigned int* seed1)
{
    glm::vec3 p;
    do
    {
        p = 2.0f * glm::vec3(getRandom(seed0, seed1), getRandom(seed0, seed1), getRandom(seed0, seed1)) - (glm::vec3)(1,1,1);
    } while (glm::dot(p,p) >= 1.0);
    return p;
}

glm::vec3 uniformSampleHemisphere(unsigned int* seed0, unsigned int* seed1)
{
    float ux = getRandom(seed0, seed1);
    float uy = getRandom(seed0, seed1);
    float z = ux;
    float r = sqrt(1.0f - z*z);
    float phi = 2*3.1415926*uy;
    float x = r * cos(phi);
    float y = r * sin(phi);
    return glm::vec3(x, y, z);
}

glm::vec3 sampleHemisphereCosine(glm::vec3 n, unsigned int* seed0, unsigned int* seed1)
{
    float phi = TWO_PI * getRandom(seed0, seed1);
    float sinThetaSqr = getRandom(seed0, seed1);
    float sinTheta = sqrt(sinThetaSqr);

    glm::vec3 axis = fabs(n.x) > 0.001f ? glm::vec3(0.0f, 1.0f, 0.0f) : glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 t = glm::normalize(glm::cross(axis, n));
    glm::vec3 s = glm::cross(n, t);
    return glm::normalize(s*cos(phi)*sinTheta + t*sin(phi)*sinTheta + n*sqrt(1.0f - sinThetaSqr));
}

unsigned int hashUInt32(unsigned int x)
{
    return 1103515245 * x + 12345;
}

int gFrameCount = 0;

void DebugEffect::update(float t)
{
	Input* input = m_context->getInput();
	Camera* camera = m_context->getCamera();
	if (input->m_mouse_button_down[1])
	{
		input->m_mouse_previou_position = input->m_mouse_position;
	}
	if (input->m_mouse_button_held[1])
	{
		camera->Rotate(input->m_mouse_position - input->m_mouse_previou_position);
		input->m_mouse_previou_position = input->m_mouse_position;
	}
	camera->Move(input->m_mouse_scroll_wheel * 5.0);

	if (input->m_mouse_button_down[0])
	{
		input->m_mouse_previou_position = input->m_mouse_position;
		glm::vec2 mouse_pos = input->m_mouse_position;
		Ray ray = pickRay(glm::vec4(0,0,m_width,m_height), mouse_pos, camera->getViewMatrix(), camera->getProjectionMatrix(m_width, m_height));
		printRayInfo(ray);
		Plane plane;
		plane.center = glm::vec3(0.0f, 0.0f, 0.0f);
		plane.normal = glm::vec3(0.0f, 0.0f, 1.0f);
		float dis = intersect(ray, plane);
		glm::vec3 hit_point = ray.pointAt(dis);
		//m_hit_points.push_back(hit_point);
        unsigned int seed0 = 0, seed1 = 0;
        seed0 = input->m_mouse_position.x + hashUInt32(gFrameCount);
        seed1 = input->m_mouse_position.y + hashUInt32(gFrameCount);;

        glm::vec3 target = hit_point + plane.normal + randomInUnitSphere(&seed0, &seed1);
        //glm::vec3 wi = target- hit_point;
        glm::vec3 wi = sampleHemisphereCosine(plane.normal, &seed0, &seed1);//uniformSampleHemisphere(&seed0, &seed1);
        m_debug_lines->addLine(&hit_point[0], &(hit_point + wi)[0], &glm::vec4(1.0, 1.0, 0.0, 0.0)[0]);
		m_debug_lines->addLine(&hit_point[0], &(hit_point + glm::vec3(1.0, 0.0, 0.0))[0], &glm::vec4(1.0, 0.0, 0.0, 0.0)[0]);
		m_debug_lines->addLine(&hit_point[0], &(hit_point + glm::vec3(0.0, 1.0, 0.0))[0], &glm::vec4(0.0, 1.0, 0.0, 0.0)[0]);
		m_debug_lines->addLine(&hit_point[0], &(hit_point + glm::vec3(0.0, 0.0, 1.0))[0], &glm::vec4(0.0, 0.0, 1.0, 0.0)[0]);
	}
}

void DebugEffect::render()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// »æÖÆdebugÏß¶Î
	GLuint debug_program = m_debug_program->getGpuProgram();
	glUseProgram(debug_program);
	glm::mat4 model = glm::mat4(1.0);
	glm::mat4 view = m_context->getCamera()->getViewMatrix();
	glm::mat4 proj = m_context->getCamera()->getProjectionMatrix(m_width, m_height);
	glUniformMatrix4fv(glGetUniformLocation(debug_program, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(debug_program, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(debug_program, "projection"), 1, GL_FALSE, &proj[0][0]);
	m_debug_lines->draw();

	// 
    for (int i = 0; i < Renderer::instance().getRenderables().size(); ++i)
    {
        std::shared_ptr<Mesh> mesh = Renderer::instance().getRenderables()[i]->getMesh();
        std::shared_ptr<Material> material = Renderer::instance().getRenderables()[i]->getMaterial();

        // drawing
        glBindVertexArray(mesh->getVertexBufferArray());
        glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
	//
	//temp::drawMesh(m_plane_mesh);

    gFrameCount++;
}

void DebugEffect::preparePlaneMesh()
{
	temp::Mesh::Vertex v0;
    temp::Mesh::Vertex v1;
    temp::Mesh::Vertex v2;
    temp::Mesh::Vertex v3;
	v0.position = glm::vec3(-1.0f, 1.0f, 0.0f);
	v0.normal = glm::vec3(1.0f, 1.0f, 1.0f);
	v0.uv = glm::vec2(0.0f, 0.0f);
	v1.position = glm::vec3(-1.0f, -1.0f, 0.0f);
	v1.normal = glm::vec3(1.0f, 1.0f, 1.0f);
	v1.uv = glm::vec2(0.0f, 0.0f);
	v2.position = glm::vec3(1.0f, 1.0f, 0.0f);
	v2.normal = glm::vec3(1.0f, 1.0f, 1.0f);
	v2.uv = glm::vec2(0.0f, 0.0f);
	v3.position = glm::vec3(1.0f, -1.0f, 0.0f);
	v3.normal = glm::vec3(1.0f, 1.0f, 1.0f);
	v3.uv = glm::vec2(0.0f, 0.0f);
	m_plane_mesh->vertices.push_back(v0);
	m_plane_mesh->vertices.push_back(v1);
	m_plane_mesh->vertices.push_back(v2);
	m_plane_mesh->vertices.push_back(v3);

	m_plane_mesh->indices.push_back(0);
	m_plane_mesh->indices.push_back(1);
	m_plane_mesh->indices.push_back(3);
	m_plane_mesh->indices.push_back(0);
	m_plane_mesh->indices.push_back(3);
	m_plane_mesh->indices.push_back(2);
}

EFFECTS_NAMESPACE_END