#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoord;
out vec2 v_texCoord;
uniform mat4 u_model;

void main()
{
    gl_Position = u_model * vec4(a_position, 1.0);
	v_texCoord = a_texCoord;
}