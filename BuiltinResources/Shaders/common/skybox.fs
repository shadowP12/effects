in vec3 v_texCoord;
uniform samplerCube u_skybox;
out vec4 FragColor;
void main()
{
    FragColor = texture(u_skybox, v_texCoord);
}