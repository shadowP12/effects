layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 6) in vec2 a_texCoords;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform sampler2D u_heightMap;
out vec3 v_worldPos;
out vec3 v_normal;
out vec2 v_texCoords;
void main()
{
	mat3 normalMatrix = transpose(inverse(mat3(u_model)));
    v_normal = normalize(normalMatrix * a_normal);
    v_texCoords = a_texCoords;
    float h = texture(u_heightMap, v_texCoords).r;
    vec4 worldPos = u_model * vec4(a_position, 1.0);
    worldPos.y += h;
    v_worldPos = worldPos.xyz;
    gl_Position = u_projection * u_view * worldPos;
}