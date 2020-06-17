in vec2 v_texCoords;
in vec3 v_normal;
in vec3 v_worldPos;

#define PI 3.1415926

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

uniform float u_roughness;
uniform float u_metallic;
#if defined(USE_METALLIC_ROUGHNESS_MAP)
uniform sampler2D u_metallicRoughnessMap;
#endif


uniform float u_aperture;
uniform float u_shutterSpeed;
uniform float u_sensitivity;
uniform vec3 u_cameraPos;
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

vec3 LinearToSRGB(vec3 linear)
{
    return pow(linear, vec3(0.454545));
}

vec3 ACESToneMap(vec3 color)
{
    const float A = 2.51;
    const float B = 0.03;
    const float C = 2.43;
    const float D = 0.59;
    const float E = 0.14;
    return (color * (A * color + B)) / (color * (C * color + D) + E);
}

vec3 fresnelSchlick(const vec3 f0, float VoH) {
    float f = pow(1.0 - VoH, 5.0);
    return f + f0 * (1.0 - f);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NoH  = max(dot(N, H), 0.0);
    float NoH2 = NoH*NoH;

    float nom   = a2;
    float denom = (NoH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NoV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NoV;
    float denom = NoV * (1.0 - k) + k;

    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NoV = max(dot(N, V), 0.0);
    float NoL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NoV, roughness);
    float ggx1  = GeometrySchlickGGX(NoL, roughness);

    return ggx1 * ggx2;
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
    float roughness = u_roughness;
    float metallic = u_metallic;
#if defined(USE_METALLIC_ROUGHNESS_MAP)
    metallic = texture(u_metallicRoughnessMap, v_texCoords).r;
    roughness = texture(u_metallicRoughnessMap, v_texCoords).g;
#endif

    float ev100 = exposureSettings(u_aperture, u_shutterSpeed, u_sensitivity);
    float exposure = exposure(ev100);
    vec3 N = normal;
    vec3 V = normalize(u_cameraPos - v_worldPos);
    vec3 L = normalize(-u_mainLitDir);
    vec3 H = normalize(L+V);
    vec3 f0 = vec3(0.56, 0.57, 0.58);
    vec3 F  = fresnelSchlick(f0, max(dot(H, V), 0.0));
    float D = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 nominator = D * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 Fr = (nominator / denominator) * metallic;
    vec3 Fd = (baseColor.rgb / 3.1415926) * (1.0 - metallic);
    vec3 color = (Fd + Fr) * u_mainLitColorIntensity.rgb * u_mainLitColorIntensity.w * exposure * saturate(dot(N, L));
    color = ACESToneMap(color);
    color = LinearToSRGB(color);
    vec4 resultColor = vec4(color, baseColor.a);

#if defined(USE_ALPHA_MODE)
    resultColor.rgb *= resultColor.a;
    float w = weight(gl_FragCoord.z, resultColor.a);
    accumColor = vec4(resultColor.rgb * w, resultColor.a);
    accumAlpha = vec4(resultColor.a * w, 0.0, 0.0, 1.0);
#else
    FragColor = resultColor;
#endif
}