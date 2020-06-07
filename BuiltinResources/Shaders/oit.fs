in vec2 v_texCoord;
uniform sampler2D u_oitTextureA;
uniform sampler2D u_oitTextureB;
out vec4 FragColor;
void main()
{
    vec4 accum = texture(u_oitTextureA, v_texCoord);
    float r = accum.a;
    accum.a = texture(u_oitTextureB, v_texCoord).r;
    FragColor = vec4(accum.rgb / clamp(accum.a, 1e-4, 5e4), r);
}