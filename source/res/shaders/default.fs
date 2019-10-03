#version 330 core

in vec2 v_texCoords;
out vec4 FragColor;
uniform sampler2D u_hightMap;
void main()
{    
	FragColor = texture(u_hightMap, v_texCoords);
}