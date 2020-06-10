layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 6) in vec2 a_texCoords;
#if defined(USE_NORMAL_MAP)
layout (location = 3) in vec3 a_tangent;
#endif

out vec2 v_texCoords;
out vec3 v_normal;
out vec3 v_worldPos;
#if defined(USE_NORMAL_MAP)
out mat3 v_tbn;
#endif

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
    v_worldPos = (u_model * vec4(a_pos, 1.0)).xyz;
    v_texCoords = a_texCoords;
	v_normal = a_normal;
	mat3 normalMatrix = transpose(inverse(mat3(u_model)));
    vec3 n = normalize(normalMatrix * a_normal);
    v_normal = n;
#if defined(USE_NORMAL_MAP)
    vec3 t = normalize(normalMatrix * a_tangent);
    vec3 b = cross(t, n);
    v_tbn = mat3(t, b, n);
#endif
}