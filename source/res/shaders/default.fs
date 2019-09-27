#version 330 core

in vec2 v_texCoords;
out vec4 FragColor;
//uniform sampler2D u_hightMap;
void main()
{    
	//float hight_data = texture(u_hightMap, v_texCoords).r;
	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}