in vec2 v_texCoords;
out vec4 FragColor;
//uniform sampler2D u_hightMap;
void main()
{   
	float r = 1.0;//texture(u_hightMap, v_texCoords).r;
	//r = r * 2.0 -1.0;
	FragColor = vec4(r, r, r, 1.0);
}