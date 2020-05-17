#version 330 core
in vec2 v_texCoords;
in vec3 v_normal;
in vec3 v_worldPos;
out vec4 FragColor;
uniform vec3 u_albedo;
uniform vec3 u_lightPos;
uniform vec3 u_lightColor;
uniform float u_lightRadius;
uniform float u_lightIntensity;
#define PI 3.1415926
#define saturate(a) clamp( a, 0.0, 1.0 )

float smoothDistanceAtt(float squaredDistance , float invSqrAttRadius)
{ 
	float factor = squaredDistance * invSqrAttRadius;
	float smoothFactor = saturate(1.0f - factor * factor);
	return smoothFactor * smoothFactor;
}

float getDistanceAtt(vec3 unormalizedLightVector , float invSqrAttRadius)
{ 
	float sqrDist = dot(unormalizedLightVector , unormalizedLightVector);
	float attenuation = 1.0 / (max(sqrDist , 0.01*0.01));
	attenuation *= smoothDistanceAtt(sqrDist , invSqrAttRadius); 
	return attenuation; 
} 

float getAngleAtt(vec3 normalizedLightVector, vec3 lightDir, float lightAngleScale , float lightAngleOffset)
{ 
	// On the CPU 
	// float lightAngleScale = 1.0f / max(0.001f, (cosInner - cosOuter)); 
	// float lightAngleOffset = -cosOuter * angleScale;
	float cd = dot(lightDir , normalizedLightVector); 
	float attenuation = saturate(cd * lightAngleScale + lightAngleOffset); 
	// smooth the transition 
	attenuation *= attenuation; 
	return attenuation; 
} 

void main()
{
    //
    vec3 lightPos = vec3(0.0, 0.1, 0.0);
	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor;
  	
    // diffuse
    vec3 N = normalize(v_normal);
    vec3 L = normalize(lightPos - v_worldPos);
    vec3 unnormalizedLightVector = lightPos - v_worldPos;
    float lightInvSqrAttRadius = 1.0 / (u_lightRadius * u_lightRadius);
    float att = 1.0;
    att *= getDistanceAtt(unnormalizedLightVector, lightInvSqrAttRadius);
    vec3 Lo = (u_albedo / PI) * saturate(dot(N, L)) * att * u_lightColor * u_lightIntensity / (4 * PI);

	vec3 result = ambient + Lo;
	FragColor = vec4(result, 1.0);
}