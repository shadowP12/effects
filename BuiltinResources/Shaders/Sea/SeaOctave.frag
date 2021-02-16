in vec2 v_texCoord;
out vec4 FragColor;

uniform float u_seaTime;

// from: https://www.shadertoy.com/view/Ms2SD1
const int ITER_GEOMETRY = 8;
const int ITER_FRAGMENT = 8;
const float SEA_HEIGHT = 0.5;
const float SEA_CHOPPY = 2.0;
const float SEA_SPEED = 0.4;
const float SEA_FREQ = 0.1;
const vec3 SEA_BASE = vec3(0.0,0.09,0.18);
const vec3 SEA_WATER_COLOR = vec3(0.8,0.9,0.6)*0.6;

float hash( vec2 p ) {
	float h = dot(p,vec2(127.1,311.7));
    return fract(sin(h)*43758.5453123);
}

float noise( in vec2 p ) {
    vec2 i = floor( p );
    vec2 f = fract( p );
	vec2 u = f*f*(3.0-2.0*f);
    return -1.0+2.0*mix( mix( hash( i + vec2(0.0,0.0) ),
                     hash( i + vec2(1.0,0.0) ), u.x),
                mix( hash( i + vec2(0.0,1.0) ),
                     hash( i + vec2(1.0,1.0) ), u.x), u.y);
}

float sea_octave(vec2 uv, float choppy) {
    vec2 uv0 = vec2(0.0);
    uv0 += noise(uv) + uv;
    vec2 wv = 1.0-abs(sin(uv0));
    vec2 swv = abs(cos(uv0));
    wv = mix(wv,swv,wv);
    return pow(1.0-pow(wv.x * wv.y,0.65),choppy);
}

float getHeight(vec2 uv, vec2 eps) {
    float freq = SEA_FREQ;
    float amp = SEA_HEIGHT;
    float choppy = SEA_CHOPPY;
    uv *= 80;
    uv.x *= 0.75;
    uv += eps;
    float d, h = 0.0;
    for(int i = 0; i < ITER_GEOMETRY; i++) {
    	d = sea_octave((uv+u_seaTime)*freq,choppy);
    	d += sea_octave((uv-u_seaTime)*freq,choppy);
        h += d * amp;
    	freq *= 1.9; amp *= 0.44;
        choppy = mix(choppy,1.0,0.2);
    }
    return h;
}

void main()
{
    float h1 = getHeight(v_texCoord, vec2(0.0));
    FragColor = vec4(h1, 0.0, 0.0, 0.0);
}