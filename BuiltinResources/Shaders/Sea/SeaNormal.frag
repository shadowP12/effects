in vec2 v_texCoord;
out vec4 FragColor;

uniform vec2 u_texelSize;
uniform sampler2D u_heightMap;

vec3 normalBlend(vec3 A, vec3 B)
{
    return normalize(vec3(A.rg + B.rg, A.b * B.b));
}

void main()
{
    vec2 shiftX = vec2(u_texelSize.x, 0.0);
    vec2 shiftZ = vec2(0.0, u_texelSize.y);

    vec4 texX = texture(u_heightMap, v_texCoord + shiftX);
    vec4 texx = texture(u_heightMap, v_texCoord - shiftX);
    vec4 texZ = texture(u_heightMap, v_texCoord + shiftZ);
    vec4 texz = texture(u_heightMap, v_texCoord - shiftZ);

    vec3 du = vec3(1.0, (texX.r - texx.r), 0.0);
    vec3 dv = vec3(0.0, (texZ.r - texz.r), 1.0);
    vec3 normal = normalize(cross(du, dv));
    normal = normal * 0.5 + 0.5;

    FragColor = vec4(normal, 1.0);
}