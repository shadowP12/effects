#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texCoords;

out vec2 v_texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(a_pos, 1.0);
    v_texCoords = a_texCoords;
}