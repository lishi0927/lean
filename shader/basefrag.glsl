#version 450 core
in vec2 TexCoord_FS_in;
in vec3 WorldPos;

uniform sampler2D colortex;
uniform sampler2D normaltex;
uniform vec3 lightdir; 
uniform vec3 cameraPos;
uniform bool ssflag;

out vec4 color;

const float texturewidth = 1024;
const float textureheight = 1024;
const float specular_pow = 100.0;
const float M_PI = 3.14159265358979323846;

const float gamma = 2.2;

vec3 pow_vec3(vec3 v, float p)
{
    return vec3(pow(v.x, p), pow(v.y, p), pow(v.z, p));
}
vec3 ToLinear(vec3 v) { return pow_vec3(v,     gamma); }
vec3 ToSRGB(vec3 v)   { return pow_vec3(v, 1.0/gamma); }

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


void main()
{
   vec3 view = normalize(cameraPos - WorldPos);
   vec3 halfv = normalize(lightdir + view);

   if(!ssflag)
   {
    vec3 normal = texture2D(normaltex, TexCoord_FS_in).xyz;
    normal = normalize(2.0 * normal - 1.0);
    color = texture2D(colortex, TexCoord_FS_in);

    float diffuse = clamp(dot(normal, lightdir), 0.0, 1.0);
    float NdotH = clamp(dot(normal, halfv), 0.0, 1.0);

    float specular = pow(clamp(NdotH , 0.0, 1.0), specular_pow) * (specular_pow) / 2 / M_PI;
   
      vec3 cd = ToLinear(vec3(0.5,0.13,0.1));
      vec3 cs = ToLinear(vec3(0.57, 0.57, 0.57));
   
      vec3 ret = diffuse * (cd + cs * specular);
	  ret = ToSRGB(ret);

	color = vec4(ret, 1.0);
   }
}