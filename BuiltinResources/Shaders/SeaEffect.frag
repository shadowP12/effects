in vec3 v_normal;
in vec2 v_texCoords;
out vec4 FragColor;

uniform vec4 u_baseColor;
#if defined(USE_BASE_COLOR_MAP)
uniform sampler2D u_baseColorMap;
#endif

#if defined(USE_DIRECT_LIGHT)
uniform vec4 u_mainLitDirection;
uniform vec4 u_mainLitColorAndIntensity;
#endif

#define PI 3.1415926
#define saturate(a) clamp( a, 0.0, 1.0 )

void main()
{
    vec4 baseColor = u_baseColor;
#if defined(USE_BASE_COLOR_MAP)
    baseColor = texture(u_baseColorMap, v_texCoords);
#endif

    vec3 color = vec3(0.0);
    color += vec3(0.1);
#if defined(USE_DIRECT_LIGHT)
    vec3 N = normalize(v_normal);
    vec3 L = normalize(-u_mainLitDirection.rgb);
    color += (baseColor.rgb / PI) * u_mainLitColorAndIntensity.rgb * u_mainLitColorAndIntensity.w * saturate(dot(N, L));
#endif
    FragColor = vec4(color, 1.0);
}