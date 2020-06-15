in vec2 v_texCoords;
in vec3 v_normal;
in vec3 v_worldPos;
#if defined(USE_NORMAL_MAP)
in mat3 v_tbn;
uniform sampler2D u_normalMap;
#endif

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

uniform vec3 u_mainLitDir;
uniform vec4 u_mainLitColorIntensity;

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
    vec3 normal = v_normal;
#if defined(USE_NORMAL_MAP)
    normal = texture(u_normalMap, v_texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(v_tbn * normal);
#endif
    vec3 lightDir = normalize(-u_mainLitDir);
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * u_mainLitColorIntensity.rgb;
    vec4 resultColor = vec4(diffuse * baseColor.rgb, baseColor.a);

#if defined(USE_ALPHA_MODE)
    resultColor.rgb *= resultColor.a;
    float w = weight(gl_FragCoord.z, resultColor.a);
    accumColor = vec4(resultColor.rgb * w, resultColor.a);
    accumAlpha = vec4(resultColor.a * w, 0.0, 0.0, 1.0);
#else
    FragColor = resultColor;
#endif
}