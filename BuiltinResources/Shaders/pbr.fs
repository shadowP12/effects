in vec2 v_texCoords;
in vec3 v_normal;
in vec3 v_worldPos;
#if defined(USE_ALPHA_MODE)
layout (location = 0) out vec4 accumColor;
layout (location = 1) out vec4 accumAlpha;
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

float weight(float z, float a) {
    return clamp(pow(min(1.0, a * 10.0) + 0.01, 3.0) * 1e8 * pow(1.0 - z * 0.9, 3.0), 1e-2, 3e3);
}

void main()
{
    vec4 baseColor = u_baseColor;
#if defined(USE_BASE_COLOR_MAP)
    baseColor = texture(u_baseColorMap, v_texCoords);
#endif
    vec4 resultColor = baseColor;
#if defined(USE_ALPHA_MODE)
    resultColor.rgb *= resultColor.a;
    float w = weight(gl_FragCoord.z, resultColor.a);
    accumColor = vec4(resultColor.rgb * w, resultColor.a);
    accumAlpha = vec4(resultColor.a * w, 0.0, 0.0, 1.0);
#else
    FragColor = resultColor;
#endif
}