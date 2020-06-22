layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 6) in vec2 a_texCoord;
out vec2 v_texCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
	v_texCoord = a_texCoord;
}