in vec2 v_texCoords;
in vec3 v_normal;
in vec3 v_worldPos;
#if defined(USE_ALPHA_MODE)
layout (location = 0) out vec4 out_oitData1;
layout (location = 1) out vec4 out_oitData2;
out vec4 FragColor;
#else
out vec4 FragColor;
#endif

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
    vec4 resultColor = baseColor;
#if defined(USE_ALPHA_MODE)
    out_oitData1 = vec4(resultColor.rgb, resultColor.w);
    out_oitData2 = vec4(1.0, 0.0, 0.0, 1.0);
#else
    FragColor = resultColor;
#endif
}