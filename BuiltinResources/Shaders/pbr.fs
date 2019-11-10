in vec2 v_texCoords;
in vec3 v_normal;
out vec4 FragColor;
uniform vec3 u_light_dir;
uniform vec3 u_light_color;

#define saturate(a) clamp( a, 0.0, 1.0 )

void main()
{   
	// ambient
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * u_light_color;
  	
    // diffuse 
    vec3 normal = normalize(v_normal);
    vec3 light_dir = normalize(u_light_dir);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * u_light_color;

	vec3 result = (ambient + diffuse);
	FragColor = vec4(result, 1.0);
}