layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
out vec4 v_color;
void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
	v_color = a_color;
}