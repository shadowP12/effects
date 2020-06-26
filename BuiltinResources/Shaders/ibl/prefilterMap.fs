out vec4 FragColor;
in vec3 v_worldPos;

#define PI 3.14159265359
uniform samplerCube u_environmentMap;
uniform float u_roughness;
uniform float u_resolution;

float RadicalInverse_VdC(uint bits) 
{
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammersley(uint i, uint N)
{
	return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
	float a = roughness*roughness;
	
	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	
	vec3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;
	
	vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent   = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);
	
	vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sampleVec);
}

void main()
{
    vec3 N = normalize(v_worldPos);
    vec3 R = N;
    vec3 V = R;
    vec3 acc = vec3(0.0);
    float accWeight = 0.0;
    uint sampleCount = 2048u;
    for(uint i = 0u; i < sampleCount; ++i)
    {
        vec2 Xi = Hammersley(i, sampleCount);
        vec3 H = ImportanceSampleGGX(Xi, N, u_roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);
        float NoL = max(dot(N, L), 0.0);
        if(NoL > 0.0)
        {
            float D   = DistributionGGX(N, H, u_roughness);
            float NoH = max(dot(N, H), 0.0);
            float HoV = max(dot(H, V), 0.0);
            float pdf = D * NoH / (4.0 * HoV) + 0.0001; 

            float omegaS = 1.0 / (float(sampleCount) * pdf + 0.0001);
            float omegaP = 4.0 * PI / (6.0 * u_resolution * u_resolution);

            float mipLevel = u_roughness == 0.0 ? 0.0 : 0.5 * log2(omegaS / omegaP); 
            
            acc       += textureLod(u_environmentMap, L, mipLevel).rgb * NoL;
            accWeight += NoL;
        }
    }
    acc = acc / accWeight;
    FragColor = vec4(acc, 1.0);
}