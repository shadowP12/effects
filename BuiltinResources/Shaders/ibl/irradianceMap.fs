out vec4 FragColor;
in vec3 v_worldPos;

#define PI 3.14159265359

uniform samplerCube u_environmentMap;

void main()
{
    vec3 normal = normalize(v_worldPos);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, normal);
    up = cross(normal, right);

    vec3 irradiance = vec3(0.0);
    float sampleDelta = 0.025;
    float sampleCount = 0.0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

            irradiance += texture(u_environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
            sampleCount++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(sampleCount));
    FragColor = vec4(irradiance, 1.0);
}