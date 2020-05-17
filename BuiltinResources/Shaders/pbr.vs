#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texCoords;
layout (location = 2) in vec3 a_normal;

out vec2 v_texCoords;
out vec3 v_normal;
out vec3 v_worldPos;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
    v_worldPos = (u_model * vec4(a_pos, 1.0)).xyz;
    v_texCoords = a_texCoords;
	v_normal = a_normal;
}