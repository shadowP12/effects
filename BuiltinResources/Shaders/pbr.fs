in vec2 v_texCoords;
in vec3 v_normal;
in vec3 v_worldPos;
out vec4 FragColor;

uniform vec4 u_baseColor;
#if defined(USE_BASE_COLOR_MAP)
uniform sampler2D u_baseColorMap;
#endif

#define PI 3.1415926
#define saturate(a) clamp( a, 0.0, 1.0 )

void main()
{
    vec4 baseColor = u_baseColor;
#if defined(USE_BASE_COLOR_MAP)
    baseColor = texture(u_baseColorMap, v_texCoords);
#endif
    FragColor = baseColor;
}