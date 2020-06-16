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

uniform float u_aperture;
uniform float u_shutterSpeed;
uniform float u_sensitivity;
uniform vec3 u_mainLitDir;
uniform vec4 u_mainLitColorIntensity;

#define PI 3.1415926
#define saturate(a) clamp( a, 0.0, 1.0 )

float exposureSettings(float aperture, float shutterSpeed, float sensitivity) {
    return log2((aperture * aperture) / shutterSpeed * 100.0 / sensitivity);
}

float exposure(float ev100) {
    return 1.0 / (pow(2.0, ev100) * 1.2);
}

// oit weight
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
    float ev100 = exposureSettings(u_aperture, u_shutterSpeed, u_sensitivity);
    float exposure = exposure(ev100);
    vec3 N = normal;
    vec3 L = normalize(-u_mainLitDir);
    vec3 Fd = baseColor.rgb / 3.1415926;
//    vec3 lightDir = normalize(-u_mainLitDir);
//    vec3 diffuse = max(dot(normal, lightDir), 0.0) * u_mainLitColorIntensity.rgb;
    vec3 color = Fd * u_mainLitColorIntensity.rgb * u_mainLitColorIntensity.w * saturate(dot(N, L));
    vec4 resultColor = vec4(color * exposure, baseColor.a);

#if defined(USE_ALPHA_MODE)
    resultColor.rgb *= resultColor.a;
    float w = weight(gl_FragCoord.z, resultColor.a);
    accumColor = vec4(resultColor.rgb * w, resultColor.a);
    accumAlpha = vec4(resultColor.a * w, 0.0, 0.0, 1.0);
#else
    FragColor = resultColor;
#endif
}