in vec3 v_worldPos;
in vec3 v_normal;
in vec2 v_texCoords;
out vec4 FragColor;

#if defined(USE_DIRECT_LIGHT)
uniform vec4 u_mainLitDirection;
uniform vec4 u_mainLitColorAndIntensity;
#endif

uniform vec3 u_viewPos;
uniform sampler2D u_normalMap;

#define PI 3.1415926
#define saturate(a) clamp( a, 0.0, 1.0 )

vec3 getSkyColor(vec3 e) {
    e.y = (max(e.y,0.0)*0.8+0.2)*0.8;
    return vec3(pow(1.0-e.y,2.0), 1.0-e.y, 0.6+(1.0-e.y)*0.4) * 1.1;
}

void main()
{
    vec4 baseColor = vec4(0.8, 0.6, 0.9, 0.8) * 0.6;

    vec3 color = vec3(0.0);
    //color += vec3(0.1);
#if defined(USE_DIRECT_LIGHT)
    vec3 normal = texture(u_normalMap, v_texCoords).rgb;
    normal = normal * 2.0 - 1.0;
    vec3 N = normalize(normal);
    vec3 L = normalize(-u_mainLitDirection.rgb);
    vec3 V = normalize(u_viewPos - v_worldPos);
    vec3 R = reflect(-V, N);
    vec3 H = normalize(L+V);
    float NoV = clamp(abs(dot(N, V))+1e-5, 0.0, 1.0);

    float fresnel = clamp(1.0 - dot(-V, N), 0.0, 1.0);
    fresnel = pow(fresnel, 5.0) * 0.05;

    vec3 reflected = getSkyColor(reflect(-V, N)) * 0.8;
    vec3 refracted = vec3(0.0,0.09,0.18) + (baseColor.rgb / PI) * u_mainLitColorAndIntensity.rgb * u_mainLitColorAndIntensity.w * saturate(dot(N, L));
    color += mix(reflected, refracted, fresnel);
#endif
    FragColor = vec4(color, 1.0);
}